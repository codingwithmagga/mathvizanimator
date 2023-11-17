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

Renderer::Renderer(QObject* parent) : QObject{parent} {}

void Renderer::render(const QList<AbstractItem*>& item_list) {
  QString program = "ffmpeg";
  QStringList arguments;
  arguments << "-y"
            << "-f"
            << "rawvideo"
            << "-video_size"
            << QString::number(m_project_settings.width) + "x" +
                   QString::number(m_project_settings.height)
            << "-pix_fmt"
            << "rgb32"
            << "-r" << QString::number(m_project_settings.fps) << "-i"
            << "-"
            << "-an"
            << "-vcodec"
            << "libx264"
            << "render_test.mp4";

  auto render_process =
      QSharedPointer<QProcess>(new RenderProcess(m_next_process_id, this));
  m_render_process_map.insert(m_next_process_id, render_process);
  m_next_process_id++;

  connect(render_process.data(), &QProcess::started, this,
          [item_list, this] { renderingProcessStarted(item_list); });
  connect(render_process.data(), &QProcess::finished, this,
          &Renderer::renderingProcessFinished);
  connect(render_process.data(), &QProcess::readyRead, this,
          [this] {
            qCInfo(renderer)
                << qobject_cast<QProcess*>(sender())->readAllStandardOutput();
          });

  render_process->setProcessChannelMode(
      QProcess::ProcessChannelMode::MergedChannels);

  render_process->start(program, arguments);
}

void Renderer::renderingProcessStarted(const QList<AbstractItem*>& item_list) {
  const qint32 num_frames =
      m_project_settings.fps * m_project_settings.video_length;

  for (qint32 frame = 0; frame < num_frames; ++frame) {
    auto image = createImage(item_list);
    auto imageData = reinterpret_cast<char*>(image.bits());

    qobject_cast<QProcess*>(sender())->write(
        imageData, m_project_settings.width * m_project_settings.height * 4);
  }

  qobject_cast<QProcess*>(sender())->closeWriteChannel();
}

void Renderer::renderingProcessFinished(qint32 exitCode,
                                        QProcess::ExitStatus exitStatus) {
  Q_UNUSED(exitCode)

  qCDebug(renderer) << "Process finished with status: " << exitStatus;
  emit finishedRendering(QFileInfo("render_test.mp4"));

  m_render_process_map.remove(qobject_cast<RenderProcess*>(sender())->id());
}

QImage Renderer::createImage(const QList<AbstractItem*>& item_list) const {
  QImage image(m_project_settings.width, m_project_settings.height,
               QImage::Format::Format_RGB32);
  image.fill("white");
  QPainter painter(&image);

  for (const auto& item : item_list) {
    item->paintItem(&painter);
  }

  return image;
}

Renderer::ProjectSettings Renderer::projectSettings() const {
  return m_project_settings;
}

void Renderer::setProjectSettings(
    const Renderer::ProjectSettings& new_project_settings) {
  m_project_settings = new_project_settings;
}
