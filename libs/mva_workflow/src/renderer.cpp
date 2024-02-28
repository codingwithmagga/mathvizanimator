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

#include "renderer.h"

#include <QImage>
#include <QPainter>

#include "logging.h"

Renderer::Renderer(QObject* parent)
    : QObject { parent }
{
}

void Renderer::render(const QList<QSharedPointer<ItemObserver>>& item_list, const QFileInfo& video_file)
{
    qCInfo(renderer) << "Rendering process requested to file " << video_file.absoluteFilePath();

    const auto project_width = QString::number(m_project_settings.size.width());
    const auto project_height = QString::number(m_project_settings.size.height());

    QString program = "ffmpeg";
    QStringList arguments;
    arguments << "-y"
              << "-f"
              << "rawvideo"
              << "-video_size" << project_width + "x" + project_height << "-pix_fmt"
              << "rgb32"
              << "-r" << QString::number(m_project_settings.fps) << "-i"
              << "-"
              << "-an"
              << "-vcodec"
              << "libx264" << video_file.absoluteFilePath();

    auto render_process = QSharedPointer<QProcess>(new RenderProcess(m_next_process_id, this));
    m_render_process_map.insert(m_next_process_id, render_process);
    m_next_process_id++;

    /* item_list needs to be captured by value(=). Capturing by reference somehow
     * breaks the animation vector in ItemObserver. This error occurred only in
     * the application, not in the unit/integration tests. I don't know why, but
     * like this it works also in the application.
     */
    connect(render_process.data(), &QProcess::started, this, [=, this] { renderingProcessStarted(item_list); });
    connect(
        render_process.data(), &QProcess::finished, this, [=, this](qint32 exitCode, QProcess::ExitStatus exitStatus) {
            renderingProcessFinished(video_file, exitCode, exitStatus);
        });
    connect(render_process.data(), &QProcess::readyRead, this,
        [this] { qCInfo(renderer) << qobject_cast<QProcess*>(sender())->readAllStandardOutput(); });

    render_process->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

    render_process->start(program, arguments);
}

void Renderer::renderingProcessStarted(const QList<QSharedPointer<ItemObserver>>& item_list)
{
    qCInfo(renderer) << "Rendering process started";

    const qint32 num_frames = m_project_settings.fps * m_project_settings.video_length;

    for (qint32 frame = 0; frame < num_frames; ++frame) {
        auto image = createImage(item_list, frame / qreal(m_project_settings.fps));
        auto imageData = reinterpret_cast<char*>(image.bits());

        qobject_cast<QProcess*>(sender())->write(
            imageData, m_project_settings.size.width() * m_project_settings.size.height() * 4);
    }

    qobject_cast<QProcess*>(sender())->closeWriteChannel();
}

void Renderer::renderingProcessFinished(const QFileInfo& video_file, qint32 exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)

    qCInfo(renderer) << "Process finished with status: " << exitStatus;
    if (exitStatus == QProcess::NormalExit) {
        qCInfo(renderer) << "Video file rendered to " << video_file.absoluteFilePath();
    }

    emit finishedRendering(video_file);

    m_render_process_map.remove(qobject_cast<RenderProcess*>(sender())->id());
}

QImage Renderer::createImage(const QList<QSharedPointer<ItemObserver>>& item_list, const qreal current_time) const
{
    QImage image(m_project_settings.size, QImage::Format::Format_RGB32);
    image.fill(m_project_settings.background_color);
  
    QPainter painter(&image);

    for (const auto& item_observer : item_list) {
        item_observer->setTimeProgressive(current_time);
        item_observer->abstractitem()->paintItem(&painter);
    }

    return image;
}

Renderer::ProjectSettings Renderer::projectSettings() const { return m_project_settings; }

void Renderer::setProjectSettings(const Renderer::ProjectSettings& new_project_settings)
{
    m_project_settings = new_project_settings;
}

void Renderer::setProjectSize(const QSize new_project_size) { m_project_settings.size = new_project_size; }

void Renderer::setFPS(const qint32 new_fps) { m_project_settings.fps = new_fps; }

void Renderer::setVideoLength(const qint32 new_video_length) { m_project_settings.video_length = new_video_length; }

void Renderer::setBackgroundColor(const QColor& new_background_color)
{
    m_project_settings.background_color = new_background_color;
}
