/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainlogic.h"

#include <QQmlContext>
#include <QStandardPaths>

#include "fade_in.h"
#include "fade_out.h"

Q_LOGGING_CATEGORY(mainlogic, "cwa.mva.mainlogic")

MainLogic::MainLogic(QObject* parent)
    : QObject { parent }
{
    m_savefilehandler.setSaveDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    connect(&m_mainwindowhandler, &MainWindowHandler::snapshotRequested, this, &MainLogic::createSnapshot);
    connect(&m_mainwindowhandler, &MainWindowHandler::renderingRequested, this, &MainLogic::renderVideo);
    connect(&m_mainwindowhandler, &MainWindowHandler::saveProjectRequested, this, &MainLogic::saveProject);
    connect(&m_mainwindowhandler, &MainWindowHandler::loadProjectRequested, this, &MainLogic::loadProject);

    connect(&m_mainwindowhandler, &MainWindowHandler::itemAdded, &m_itemhandler, &ItemHandler::addItem);
    connect(&m_mainwindowhandler, &MainWindowHandler::removeCurrentItemRequested, &m_itemhandler,
        &ItemHandler::removeCurrentItem);
    connect(&m_mainwindowhandler, &MainWindowHandler::removeAnimationRequested, &m_itemhandler,
        &ItemHandler::removeAnimation);
    connect(&m_mainwindowhandler, &MainWindowHandler::itemClicked, &m_itemhandler, &ItemHandler::setCurrentItem);
    connect(&m_mainwindowhandler, &MainWindowHandler::propertyChanged, &m_itemhandler, &ItemHandler::changeProperty);
    connect(&m_mainwindowhandler, &MainWindowHandler::newProjectRequested, &m_itemhandler, &ItemHandler::clear);
    connect(&m_mainwindowhandler, &MainWindowHandler::addAnimationSignal, &m_itemhandler, &ItemHandler::addAnimation);

    connect(&m_mainwindowhandler, &MainWindowHandler::projectSizeChanged, this, &MainLogic::projectSizeChanged);
    connect(&m_mainwindowhandler, &MainWindowHandler::fpsChanged, &m_renderer, &Renderer::setFPS);
    connect(&m_mainwindowhandler, &MainWindowHandler::videoLengthChanged, &m_renderer, &Renderer::setVideoLength);
    connect(
        &m_mainwindowhandler, &MainWindowHandler::backgroundColorChanged, &m_renderer, &Renderer::setBackgroundColor);

    connect(&m_mainwindowhandler, &MainWindowHandler::timeChanged, this, &MainLogic::uiTimeChanged);

    connect(
        &m_renderer, &Renderer::finishedRendering, &m_mainwindowhandler, &MainWindowHandler::renderingVideoFinished);
    connect(&m_renderer, &Renderer::finishedRendering, this, &MainLogic::renderingVideoFinished);

    const auto default_project_settings = m_renderer.projectSettings();
    QList<qint32> conv_project_settings { default_project_settings.size.width(), default_project_settings.size.height(),
        default_project_settings.fps, default_project_settings.video_length };
    m_mainwindowhandler.updateProjectSettings(conv_project_settings, default_project_settings.background_color);
}

void MainLogic::initEngine(QQmlApplicationEngine* const engine)
{
    m_qml_engine = engine;

    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("main_window"), &m_mainwindowhandler);
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("item_model"), m_itemhandler.model());
    m_qml_engine->rootContext()->setContextProperty(
        QStringLiteral("item_selection_model"), m_itemhandler.selectionModel());
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("property_model"), m_itemhandler.propertyModel());
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("animation_model"), m_itemhandler.animationModel());
}

void MainLogic::connectEngine()
{
    const auto rootObjects = m_qml_engine->rootObjects();

    if (rootObjects.isEmpty()) {
        qCCritical(mainlogic) << "No root objects found in current qml engine. Init aborted.";
        return;
    }

    m_qml_creation_area = rootObjects.first()->findChild<QObject*>("MVACreationArea");

    if (!m_qml_creation_area) {
        qCCritical(mainlogic) << "Can't find creation area for videos. Init aborted.";
        return;
    }

    // clang-format off
    QObject::connect(m_qml_creation_area, SIGNAL(itemAdded(QQuickItem*)), &m_mainwindowhandler,
                     SLOT(addItem(QQuickItem*)));
    // clang-format on
}

void MainLogic::createSnapshot(const QFileInfo& snapshot_file_info)
{
    auto item_list = m_itemhandler.items();
    const auto snapshot = m_renderer.createImage(item_list, m_current_time);

    snapshot.save(snapshot_file_info.absoluteFilePath());
}

void MainLogic::renderVideo(const QFileInfo& video_file_info)
{
    m_itemhandler.setTime(0.0);
    auto item_list = m_itemhandler.items();
    m_renderer.render(item_list, video_file_info);
}

void MainLogic::saveProject(const QFileInfo& save_file_info)
{
    QJsonObject save_json;
    qint32 count = 0;
    m_itemhandler.setTime(0.0);
    const auto item_observer_list = m_itemhandler.items();

    for (const auto& item_observer : item_observer_list) {
        const auto json = item_observer->toJson();
        save_json["item_" + QString::number(count)] = json;
        count++;
    }

    save_json.insert("mva-version", MVA_PROJECT_VERSION);
    save_json.insert("project-settings", projectSettingsJson());

    m_savefilehandler.setSaveDir(save_file_info.absoluteDir());
    m_savefilehandler.saveJSON(save_file_info.fileName(), save_json);
    m_itemhandler.setTime(m_current_time);
}

void MainLogic::loadProject(const QFileInfo& load_file_info)
{
    QJsonDocument loadDoc = m_savefilehandler.loadJSON(load_file_info);

    QJsonObject json = loadDoc.object();

    setProjectSettings(json["project-settings"].toObject());

    for (const QString& itemKey : json.keys()) {
        if (!itemKey.startsWith("item_")) {
            continue;
        }
        auto item_json = json[itemKey].toObject();

        QQmlComponent component(m_qml_engine, QUrl(item_json["abstract_item.file"].toString()));

        QList<QSharedPointer<AbstractAnimation>> animations;
        const auto keys = item_json.keys();
        for (const auto& key : keys) {
            if (key.startsWith("animation_")) {
                auto animation = item_json[key].toObject();
                const auto animation_type = animation["type"].toString();
                animation.remove("type");

                const auto animation_properties = animation.toVariantMap();

                if (animation_type == "FadeIn") {
                    auto fade_in = QSharedPointer<AbstractAnimation>(new FadeIn);
                    fade_in->setProperties(animation_properties);
                    animations.append(fade_in);
                } else if (animation_type == "FadeOut") {
                    auto fade_out = QSharedPointer<AbstractAnimation>(new FadeOut);
                    fade_out->setProperties(animation_properties);
                    animations.append(fade_out);
                } else {
                    qCWarning(mainlogic) << "Animation" << animation_type << "unknown!";
                }
                item_json.remove(key);
            }
        }

        auto item_properties = item_json.toVariantMap();

        // "file" is a read-only property which will be set when the item is
        // created. It is not possible (or necessary) to set it in this context.
        item_properties.remove("abstract_item.file");

        item_properties.insert("parent", QVariant::fromValue(m_qml_creation_area));
        QObject* comp = component.createWithInitialProperties(item_properties);
        if (!comp) {
            qCWarning(mainlogic) << "component not found!";
        }
        BasicItem* item = qobject_cast<BasicItem*>(comp);
        if (!item) {
            qCWarning(mainlogic) << "item not found!";
        }

        m_mainwindowhandler.addItem(item);
        m_itemhandler.addAnimations(item_properties.value("abstract_item.name").toString(), animations);
    }
}

void MainLogic::projectSizeChanged()
{
    const auto project_size = m_mainwindowhandler.projectSize();
    const auto previous_project_size = m_renderer.projectSettings().size;

    const auto width_ratio = project_size.width() / qreal(previous_project_size.width());
    const auto height_ratio = project_size.height() / qreal(previous_project_size.height());

    m_itemhandler.scaleItems(width_ratio, height_ratio);
    m_renderer.setProjectSize(project_size);
}

void MainLogic::uiTimeChanged(const qreal time)
{
    m_itemhandler.setTime(time);
    m_current_time = time;
}

void MainLogic::renderingVideoFinished() { uiTimeChanged(m_current_time); }

QJsonObject MainLogic::projectSettingsJson() const
{
    QJsonObject json;

    const auto current_project_settings = m_renderer.projectSettings();

    json.insert("width", current_project_settings.size.width());
    json.insert("height", current_project_settings.size.height());
    json.insert("fps", current_project_settings.fps);
    json.insert("video_length", current_project_settings.video_length);
    json.insert("background_color", current_project_settings.background_color.name());

    return json;
}

void MainLogic::setProjectSettings(const QJsonObject& json)
{
    m_mainwindowhandler.setProjectSize(QSize(json["width"].toInt(), json["height"].toInt()));
    m_mainwindowhandler.setFPS(json["fps"].toInt());
    m_mainwindowhandler.setVideoLength(json["video_length"].toInt());
    m_mainwindowhandler.setBackgroundColor(json["background_color"].toVariant().value<QColor>());
}
