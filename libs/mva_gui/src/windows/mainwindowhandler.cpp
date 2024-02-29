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

MainWindowHandler::MainWindowHandler(QObject* parent)
    : QObject(parent)
{
}

void MainWindowHandler::snapshot(const QVariant& file)
{
    QFileInfo snapshot_file_info(file.toUrl().toLocalFile());
    emit snapshotRequested(snapshot_file_info);
}

void MainWindowHandler::render(const QVariant& file)
{
    QFileInfo video_file_info(file.toUrl().toLocalFile());
    emit renderingRequested(video_file_info);
}

void MainWindowHandler::saveProject(const QVariant& file)
{
    QFileInfo save_file_info(file.toUrl().toLocalFile());
    emit saveProjectRequested(save_file_info);
}

void MainWindowHandler::loadProject(const QVariant& file)
{
    QFileInfo load_file_info(file.toUrl().toLocalFile());

    qCInfo(mainwindow_handler) << "Start loading project from file" << load_file_info.absoluteFilePath();

    emit loadProjectRequested(load_file_info);
}

void MainWindowHandler::removeCurrentItem() { emit removeCurrentItemRequested(); }

void MainWindowHandler::removeAnimation(const qint32 animation_number)
{
    emit removeAnimationRequested(animation_number);
}

void MainWindowHandler::itemClickedByUser(const QString& item_name) { emit itemClicked(item_name); }

void MainWindowHandler::setTimeByUser(const QVariant& time) { emit timeChanged(time.toDouble()); }

void MainWindowHandler::propertyChangedByUser(
    const QString& item_name, const QByteArray& property, const QVariant& value)
{
    emit propertyChanged(item_name, property, value);
}

void MainWindowHandler::updateProjectSettings(const QVariantList& new_project_settings)
{
    if (new_project_settings.size() != 5) {
        qCCritical(mainwindow_handler) << "Internal error. Wrong number of project settings delivered.";
        return;
    }

    QList<qint32> conv_project_settings { new_project_settings[0].toInt(), new_project_settings[1].toInt(),
        new_project_settings[2].toInt(), new_project_settings[3].toInt() };
    updateProjectSettings(conv_project_settings, new_project_settings[4].value<QColor>());
}

void MainWindowHandler::updateProjectSettings(
    const QList<qint32>& new_project_settings, const QColor& new_background_color)
{
    if (new_project_settings.size() != 4) {
        qCCritical(mainwindow_handler) << "Internal error. Wrong number of project settings delivered.";
        return;
    }

    setProjectSize(QSize(new_project_settings[0], new_project_settings[1]));
    setFPS(new_project_settings[2]);
    setVideoLength(new_project_settings[3]);
    setBackgroundColor(new_background_color);
}

void MainWindowHandler::newProject()
{
    qCInfo(mainwindow_handler) << "A new Project is started.";
    emit newProjectRequested();
}

qint32 MainWindowHandler::fps() const { return m_fps; }

qint32 MainWindowHandler::videoLength() const { return m_video_length; }

void MainWindowHandler::setFPS(const qint32 new_fps)
{
    if (m_fps == new_fps) {
        return;
    }

    m_fps = new_fps;
    emit fpsChanged(new_fps);
}

void MainWindowHandler::setVideoLength(const qint32 new_video_length)
{
    if (m_video_length == new_video_length) {
        return;
    }

    m_video_length = new_video_length;
    emit videoLengthChanged(new_video_length);
}

void MainWindowHandler::addAnimation(
    const QString& item_name, const QString& animation_type, const qreal start_time, const qreal duration)
{
    emit addAnimationSignal(item_name, animation_type, start_time, duration);
}

void MainWindowHandler::openSVGFolder() const
{
    qCInfo(mainwindow_handler) << "Opening SVG Folder successful: "
                               << QDesktopServices::openUrl(
                                      QUrl(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
}

void MainWindowHandler::addItem(QQuickItem* item)
{
    qCInfo(mainwindow_handler) << "addItem";
    auto basic_item = dynamic_cast<BasicItem*>(item);
    if (!basic_item) {
        qCWarning(mainwindow_handler)
            << "Added item is not a BasicItem (or derived from it). Item can't be added to Project.";
        return;
    }

    connect(basic_item, &BasicItem::clicked, this, &MainWindowHandler::itemClickedByUser);
    connect(basic_item, &BasicItem::animationAdded, this, &MainWindowHandler::addAnimation);

    emit itemAdded(basic_item);
}

QSize MainWindowHandler::projectSize() const { return m_project_size; }

void MainWindowHandler::setProjectSize(const QSize& new_project_size)
{
    if (m_project_size == new_project_size) {
        return;
    }

    m_project_size = new_project_size;
    emit projectSizeChanged();
}

QColor MainWindowHandler::backgroundColor() const { return m_background_color; }

void MainWindowHandler::setBackgroundColor(const QColor& newBackgroundColor)
{
    if (m_background_color == newBackgroundColor) {
        return;
    }

    m_background_color = newBackgroundColor;
    emit backgroundColorChanged(m_background_color);
}
