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

#ifndef LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_RENDERER_H_
#define LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_RENDERER_H_

#include <QFileInfo>
#include <QObject>
#include <QProcess>

#include "abstractitem.h"

class Renderer : public QObject {
  Q_OBJECT

 public:
  struct ProjectSettings {
    qint32 width = 1024;
    qint32 height = 768;
    qint32 fps = 24;
    qint32 video_length = 5;
  };

  explicit Renderer(QObject* parent = nullptr);

  ProjectSettings projectSettings() const;
  void setProjectSettings(const ProjectSettings& new_project_settings);

 public slots:
  void render(const QList<AbstractItem*>& item_list);

 signals:
  void finishedRendering(const QFileInfo& video_file);

 private slots:
  void renderingProcessStarted(const QList<AbstractItem*>& item_list);
  void renderingProcessFinished(qint32 exitCode,
                                QProcess::ExitStatus exitStatus);

 private:
  QProcess m_render_process;
  ProjectSettings m_project_settings;
};

#endif  // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_RENDERER_H_
