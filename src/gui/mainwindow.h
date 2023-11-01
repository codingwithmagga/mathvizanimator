/* mathvizanimator
 * Copyright (C) 2023  codingwithmagga
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QStandardItemModel>

#include "itemhandler.h"
#include "renderer.h"
#include "savefilehandler.h"

class MainWindowHandler : public QObject {
  Q_OBJECT

  Q_PROPERTY(qint32 pixel_width READ pixelWidth WRITE setPixelWidth NOTIFY
                 pixelWidthChanged)
  Q_PROPERTY(qint32 pixel_height READ pixelHeight WRITE setPixelHeight NOTIFY
                 pixelHeightChanged)
  Q_PROPERTY(qint32 fps READ fps WRITE setFPS NOTIFY fpsChanged)
  Q_PROPERTY(qint32 video_length READ videoLength WRITE setVideoLength NOTIFY
                 videoLengthChanged)

 public:
  MainWindowHandler();

  void init();
  void initEngine(QQmlApplicationEngine *const engine);

 public slots:

  void render();

  void save(const QVariant &file);
  void load(const QVariant &file);

  void addItem(QQuickItem *quick_item);
  void removeItem(QQuickItem *quick_item);
  void removeRow(const int row);

  int getRowByItemName(QVariant name);

  void updateProjectSettings(QVariantList newProjectSettings);

  void clearAllItems();

  void currentRowChanged(const int row);

  qint32 pixelWidth() const;
  qint32 pixelHeight() const;
  qint32 fps() const;
  qint32 videoLength() const;

  void setPixelWidth(qint32 new_pixel_width);
  void setPixelHeight(qint32 new_pixel_height);
  void setFPS(qint32 new_fps);
  void setVideoLength(qint32 new_video_length);

  void openSVGFolder() const;

 signals:
  void pixelWidthChanged(const qint32 new_pixel_width);
  void pixelHeightChanged(const qint32 new_pixel_height);
  void fpsChanged(const qint32 new_fps);
  void videoLengthChanged(const qint32 new_video_length);

 private:
  QQmlApplicationEngine *m_qml_engine;
  QObject *m_qml_creation_area;

  SaveFileHandler m_savefile_handler;
  Renderer m_renderer;
  ItemHandler m_itemhandler;
};

#endif  // MAINWINDOW_H
