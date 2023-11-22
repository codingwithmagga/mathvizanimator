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

#ifndef APP_SRC_GUI_MAINWINDOWHANDLER_H_
#define APP_SRC_GUI_MAINWINDOWHANDLER_H_

#include <QFileInfo>
#include <QObject>
#include <QQuickItem>

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
  MainWindowHandler(QObject *parent = Q_NULLPTR);

  qint32 pixelWidth() const;
  qint32 pixelHeight() const;
  qint32 fps() const;
  qint32 videoLength() const;

 public slots:

  void snapshot();
  void render();

  void newProject();
  void saveProject(const QVariant &file);
  void loadProject(const QVariant &file);

  void removeCurrentItem();

  void itemClickedByUser(const QVariant &itemName);

  void updateProjectSettings(const QVariantList &new_project_settings);
  void updateProjectSettings(const QList<qint32> &new_project_settings);

  void setPixelWidth(const qint32 new_pixel_width);
  void setPixelHeight(const qint32 new_pixel_height);
  void setFPS(const qint32 new_fps);
  void setVideoLength(const qint32 new_video_length);

  void openSVGFolder() const;

 signals:
  void pixelWidthChanged(const qint32 new_pixel_width);
  void pixelHeightChanged(const qint32 new_pixel_height);
  void fpsChanged(const qint32 new_fps);
  void videoLengthChanged(const qint32 new_video_length);

  void snapshotRequested();
  void renderingRequested();

  void saveProjectRequested(const QFileInfo &savefileinfo);
  void loadProjectRequested(const QFileInfo &savefileinfo);
  void newProjectRequested();

  void removeCurrentItemRequested();

  void itemClicked(const QString &name);

 private:
  QObject *m_qml_creation_area;

  qint32 m_pixel_width;
  qint32 m_pixel_height;
  qint32 m_fps;
  qint32 m_video_length;
};

#endif  // APP_SRC_GUI_MAINWINDOWHANDLER_H_
