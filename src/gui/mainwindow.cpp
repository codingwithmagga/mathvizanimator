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

#include "mainwindow.h"

#include <QDesktopServices>
#include <QList>
#include <QQmlContext>
#include <QStandardPaths>

#include "abstractitem.h"
#include "logging.h"

MainWindowHandler::MainWindowHandler() {
  m_savefile_handler.setSaveDir(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

void MainWindowHandler::initEngine(QQmlApplicationEngine *const engine) {
  m_qml_engine = engine;

  m_qml_engine->rootContext()->setContextProperty(QStringLiteral("main_window"),
                                                  this);
  m_qml_engine->rootContext()->setContextProperty(QStringLiteral("item_model"),
                                                  m_itemhandler.model());
  m_qml_engine->rootContext()->setContextProperty(
      QStringLiteral("property_model"), m_itemhandler.propertyModel());
}

void MainWindowHandler::init() {
  const auto rootObjects = m_qml_engine->rootObjects();

  if (rootObjects.isEmpty()) {
    qCCritical(mainwindow_handler)
        << "No root objects found in current qml engine. Init aborted.";
    return;
  }

  m_qml_creation_area =
      rootObjects.first()->findChild<QObject *>("creationArea");

  if (!m_qml_creation_area) {
    qCCritical(mainwindow_handler)
        << "Can't find creation area for videos. Init aborted.";
    return;
  }

  // clang-format off
            QObject::connect(m_qml_creation_area,
                             SIGNAL(itemAdded(QQuickItem*)),
                             this,
                             SLOT(addItem(QQuickItem*)));
  // clang-format on
}

void MainWindowHandler::render() {
  QList<AbstractItem *> abstractitem_list;

  const auto item_list = m_itemhandler.items();
  for (const auto &item : item_list) {
    abstractitem_list.push_back(
        qvariant_cast<AbstractItem *>(item->property("item")));
  }
  m_renderer.render(abstractitem_list);
}

void MainWindowHandler::save(const QVariant &file) {
  QJsonObject save_json;
  qint32 count = 0;
  QString element_prefix = "element_";
  const auto item_list = m_itemhandler.items();

  for (const auto &item : item_list) {
    const auto abstract_item =
        qvariant_cast<AbstractItem *>(item->property("item"));

    const auto json_element = abstract_item->toJson();
    save_json[element_prefix + QString::number(count)] = json_element;
    count++;
  }

  QFileInfo save_fileinfo(file.toUrl().toLocalFile());
  m_savefile_handler.setSaveDir(save_fileinfo.absoluteDir());
  m_savefile_handler.saveJSON(save_fileinfo.fileName(), save_json);
}

void MainWindowHandler::load(const QVariant &file) {
  QFileInfo load_fileinfo(file.toUrl().toLocalFile());
  QJsonDocument loadDoc = m_savefile_handler.loadJSON(load_fileinfo);

  QJsonObject json = loadDoc.object();
  for (const QString &elementKey : json.keys()) {
    auto element = json[elementKey].toObject();

    QQmlComponent component(m_qml_engine,
                            QUrl(element["item.file"].toString()));

    auto elementProperties = element.toVariantMap();

    // "file" is a read-only property which will be set when the item is
    // created. It is not possible (or necessary) to set it in this context.
    elementProperties.remove("item.file");

    elementProperties.insert("parent",
                             QVariant::fromValue(m_qml_creation_area));
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

void MainWindowHandler::addItem(QQuickItem *quick_item) {
  m_itemhandler.addItem(quick_item);
}

void MainWindowHandler::removeItem(QQuickItem *quick_item) {
  m_itemhandler.removeItem(quick_item);
}

void MainWindowHandler::removeRow(const qint32 row) {
  if (row == -1) {
    return;
  }

  m_itemhandler.model()->removeRow(row);
}

qint32 MainWindowHandler::getRowByItemName(QVariant name) {
  const auto itemName = name.toString();

  const auto itemList = m_itemhandler.model()->findItems(itemName);

  if (itemList.isEmpty()) {
    qCCritical(rendering) << "Can't find clicked item. This shouldn't happen!";
    return -1;
  }

  return itemList.at(0)->row();
}

void MainWindowHandler::updateProjectSettings(QVariantList newProjectSettings) {
  if (newProjectSettings.size() != 4) {
    qCCritical(mainwindow_handler)
        << "Internal error. Wrong number of project settings delivered.";
    return;
  }

  setPixelWidth(newProjectSettings[0].toInt());
  setPixelHeight(newProjectSettings[1].toInt());
  setFPS(newProjectSettings[2].toInt());
  setVideoLength(newProjectSettings[3].toInt());
}

void MainWindowHandler::clearAllItems() { m_itemhandler.clear(); }

void MainWindowHandler::currentRowChanged(const qint32 row) {
  m_itemhandler.setCurrentRow(row);
}

qint32 MainWindowHandler::pixelWidth() const {
  return m_renderer.projectSettings().width;
}

qint32 MainWindowHandler::pixelHeight() const {
  return m_renderer.projectSettings().height;
}

qint32 MainWindowHandler::fps() const {
  return m_renderer.projectSettings().fps;
}

qint32 MainWindowHandler::videoLength() const {
  return m_renderer.projectSettings().video_length;
}

void MainWindowHandler::setPixelWidth(qint32 new_pixel_width) {
  auto projectSettings = m_renderer.projectSettings();

  if (projectSettings.width == new_pixel_width) {
    return;
  }

  const qreal ratio = new_pixel_width / qreal(projectSettings.width);
  m_itemhandler.scaleItemsX(ratio);
  m_itemhandler.scaleItemsWidth(ratio);

  projectSettings.width = new_pixel_width;
  m_renderer.setProjectSettings(projectSettings);

  emit pixelWidthChanged(new_pixel_width);
}

void MainWindowHandler::setPixelHeight(qint32 new_pixel_height) {
  auto projectSettings = m_renderer.projectSettings();

  if (projectSettings.height == new_pixel_height) {
    return;
  }

  const qreal ratio = new_pixel_height / qreal(projectSettings.height);
  m_itemhandler.scaleItemsY(ratio);
  m_itemhandler.scaleItemsHeight(ratio);

  projectSettings.height = new_pixel_height;
  m_renderer.setProjectSettings(projectSettings);

  emit pixelHeightChanged(new_pixel_height);
}

void MainWindowHandler::setFPS(qint32 new_fps) {
  auto projectSettings = m_renderer.projectSettings();

  if (projectSettings.fps == new_fps) {
    return;
  }

  projectSettings.fps = new_fps;
  m_renderer.setProjectSettings(projectSettings);

  emit pixelWidthChanged(new_fps);
}

void MainWindowHandler::setVideoLength(qint32 new_video_length) {
  auto projectSettings = m_renderer.projectSettings();

  if (projectSettings.video_length == new_video_length) {
    return;
  }

  projectSettings.video_length = new_video_length;
  m_renderer.setProjectSettings(projectSettings);

  emit videoLengthChanged(new_video_length);
}

void MainWindowHandler::openSVGFolder() const {
  QDesktopServices::openUrl(
      QUrl(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
}
