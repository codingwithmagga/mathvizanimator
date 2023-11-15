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
            << "bgra"
            << "-r" << QString::number(m_project_settings.fps) << "-i"
            << "-"
            << "-an"
            << "-vcodec"
            << "libx264"
            << "render_test.mp4";

  connect(&m_render_process, &QProcess::started, this,
          [item_list, this] { renderingProcessStarted(item_list); });
  connect(&m_render_process, &QProcess::finished, this,
          &Renderer::renderingProcessFinished);
  connect(&m_render_process, &QProcess::readyRead, this, [this] {
    qCInfo(renderer) << m_render_process.readAllStandardOutput();
  });

  m_render_process.setProcessChannelMode(
      QProcess::ProcessChannelMode::MergedChannels);

  m_render_process.start(program, arguments);
}

void Renderer::renderingProcessStarted(const QList<AbstractItem*>& item_list) {
  const int num_frames =
      m_project_settings.fps * m_project_settings.video_length;

  for (int frame = 0; frame < num_frames; ++frame) {
    QImage image(m_project_settings.width, m_project_settings.height,
                 QImage::Format::Format_ARGB32);
    image.fill("white");
    QPainter painter(&image);

    for (const auto& item : item_list) {
      painter.save();
      painter.translate(item->parentItem()->position());
      painter.setOpacity(item->opacity());
      if (item->rotation() != 0) {
        QPointF item_middle_point(item->width() / 2.0, item->height() / 2.0);
        painter.translate(item_middle_point);
        painter.rotate(item->rotation());
        painter.translate(-item_middle_point);
      }
      item->paint(&painter);
      painter.restore();
    }

    auto imageData = reinterpret_cast<char*>(image.bits());
    m_render_process.write(
        imageData, m_project_settings.width * m_project_settings.height * 4);
  }

  m_render_process.closeWriteChannel();
}

void Renderer::renderingProcessFinished(int exitCode,
                                        QProcess::ExitStatus exitStatus) {
  Q_UNUSED(exitCode)

  qCDebug(renderer) << "Process finished with status: " << exitStatus;
  emit finishedRendering(QFileInfo("render_test.mp4"));
}

Renderer::ProjectSettings Renderer::projectSettings() const {
  return m_project_settings;
}

void Renderer::setProjectSettings(
    const Renderer::ProjectSettings& new_project_settings) {
  m_project_settings = new_project_settings;
}
