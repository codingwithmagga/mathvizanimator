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

#include "mainwindowhandler.h"

#include <QDesktopServices>
#include <QStandardPaths>

#include "logging.h"

MainWindowHandler::MainWindowHandler(QObject *parent) : QObject(parent) {}

void MainWindowHandler::snapshot() { emit snapshotRequested(); }

void MainWindowHandler::render() { emit renderingRequested(); }

void MainWindowHandler::saveProject(const QVariant &file) {
  QFileInfo savefileinfo(file.toUrl().toLocalFile());
  emit saveProjectRequested(savefileinfo);
}

void MainWindowHandler::loadProject(const QVariant &file) {
  QFileInfo loadfileinfo(file.toUrl().toLocalFile());
  emit loadProjectRequested(loadfileinfo);
}

void MainWindowHandler::removeCurrentItem() {
  emit removeCurrentItemRequested();
}

void MainWindowHandler::itemClickedByUser(const QVariant &itemName) {
  emit itemClicked(itemName.toString());
}

void MainWindowHandler::updateProjectSettings(
    const QVariantList &new_project_settings) {
  if (new_project_settings.size() != 4) {
    qCCritical(mainwindow_handler)
        << "Internal error. Wrong number of project settings delivered.";
    return;
  }

  QList<qint32> conv_project_settings{
      new_project_settings[0].toInt(), new_project_settings[1].toInt(),
      new_project_settings[2].toInt(), new_project_settings[3].toInt()};
  updateProjectSettings(conv_project_settings);
}

void MainWindowHandler::updateProjectSettings(
    const QList<qint32> &new_project_settings) {
  if (new_project_settings.size() != 4) {
    qCCritical(mainwindow_handler)
        << "Internal error. Wrong number of project settings delivered.";
    return;
  }

  setPixelWidth(new_project_settings[0]);
  setPixelHeight(new_project_settings[1]);
  setFPS(new_project_settings[2]);
  setVideoLength(new_project_settings[3]);
}

void MainWindowHandler::newProject() { emit newProjectRequested(); }

qint32 MainWindowHandler::pixelWidth() const { return m_pixel_width; }

qint32 MainWindowHandler::pixelHeight() const { return m_pixel_height; }

qint32 MainWindowHandler::fps() const { return m_fps; }

qint32 MainWindowHandler::videoLength() const { return m_video_length; }

void MainWindowHandler::setPixelWidth(const qint32 new_pixel_width) {
  if (m_pixel_width == new_pixel_width) {
    return;
  }

  m_pixel_width = new_pixel_width;
  emit pixelWidthChanged(new_pixel_width);
}

void MainWindowHandler::setPixelHeight(const qint32 new_pixel_height) {
  if (m_pixel_height == new_pixel_height) {
    return;
  }

  m_pixel_height = new_pixel_height;
  emit pixelHeightChanged(new_pixel_height);
}

void MainWindowHandler::setFPS(const qint32 new_fps) {
  if (m_fps == new_fps) {
    return;
  }

  m_fps = new_fps;
  emit fpsChanged(new_fps);
}

void MainWindowHandler::setVideoLength(const qint32 new_video_length) {
  if (m_video_length == new_video_length) {
    return;
  }

  m_video_length = new_video_length;
  emit videoLengthChanged(new_video_length);
}

void MainWindowHandler::openSVGFolder() const {
  QDesktopServices::openUrl(
      QUrl(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
}
