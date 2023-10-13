#include "mainwindow.h"

#include <iostream>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QQmlContext>
#include <QStandardPaths>
#include <QThread>

#include "abstractitem.h"
#include "logging.h"

MainWindowHandler::MainWindowHandler(QQmlApplicationEngine *const engine)
    : m_qml_engine(engine)
{
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("main_window"), this);
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("item_model"),
                                                    m_itemhandler.model());

    m_savefile_handler.setSaveDir(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

void MainWindowHandler::init()
{
    const auto rootObjects = m_qml_engine->rootObjects();

    if (rootObjects.isEmpty()) {
        qCCritical(mainwindow_handler)
            << "No root objects found in current qml engine. Init aborted.";
        return;
    }

    m_qml_creation_area = rootObjects.first()->findChild<QObject *>("creationArea");

    if (!m_qml_creation_area) {
        qCCritical(mainwindow_handler) << "Can't find creation area for videos. Init aborted.";
        return;
    }

    // clang-format off
    QObject::connect(m_qml_creation_area,
                     SIGNAL(itemAdded(QQuickItem*)),
                     this,
                     SLOT(addItem(QQuickItem*)));
    // clang-format on
}

void MainWindowHandler::render()
{
    QList<AbstractItem *> abstractitem_list;

    const auto item_list = m_itemhandler.items();
    for (const auto &item : item_list) {
        abstractitem_list.push_back(qvariant_cast<AbstractItem *>(item->property("item")));
    }
    m_renderer.render(abstractitem_list);
}

void MainWindowHandler::save(const QVariant &file)
{
    QJsonObject save_json;
    int count = 0;
    QString element_prefix = "element_";
    const auto item_list = m_itemhandler.items();

    for (const auto &item : item_list) {
        const auto qobj = qvariant_cast<AbstractItem *>(item->property("item"));

        const auto json_element = qobj->toJson();
        save_json[element_prefix + QString::number(count)] = json_element;
        count++;
    }

    QFileInfo save_fileinfo(file.toUrl().toLocalFile());
    m_savefile_handler.setSaveDir(save_fileinfo.absoluteDir());
    m_savefile_handler.saveJSON(save_fileinfo.fileName(), save_json);
}

// TODO: Issue when loading data, items can't be selected anymore
// Also _1 is appended every time.
void MainWindowHandler::load(const QVariant &file)
{
    QFileInfo load_fileinfo(file.toUrl().toLocalFile());
    QJsonDocument loadDoc = m_savefile_handler.loadJSON(load_fileinfo);

    QJsonObject json = loadDoc.object();
    foreach (const QString &elementKey, json.keys()) {
        auto element = json[elementKey].toObject();

        QQmlComponent component(m_qml_engine, QUrl(element["item.file"].toString()));

        auto elementProperties = element.toVariantMap();
        elementProperties.insert("parent", QVariant::fromValue(m_qml_creation_area));
        QObject *comp = component.createWithInitialProperties(elementProperties);
        if (!comp) {
            qWarning("comp not found!");
        }
        QQuickItem *item = qobject_cast<QQuickItem *>(comp);
        if (!item) {
            qWarning("item  not found!");
        }

        addItem(item);
    }
}

void MainWindowHandler::addItem(QQuickItem *quick_item)
{
    m_itemhandler.addItem(quick_item);
}

void MainWindowHandler::removeItem(QQuickItem *quick_item)
{
    m_itemhandler.removeItem(quick_item);
}

void MainWindowHandler::removeRow(const int row)
{
    if (row == -1) {
        return;
    }

    m_itemhandler.model()->removeRow(row);
}

int MainWindowHandler::getRowByItemName(QVariant name)
{
    const auto itemName = name.toString();

    const auto itemList = m_itemhandler.model()->findItems(itemName);

    if (itemList.isEmpty()) {
        qCCritical(rendering) << "Can't find clicked item. This shouldn't happen!";
        return -1;
    }

    return itemList.at(0)->row();
}

void MainWindowHandler::clearAllItems()
{
    m_itemhandler.clear();
}
