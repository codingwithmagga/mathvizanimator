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
#include <QSharedPointer>

#include "item_observer.h"

class Renderer : public QObject {
    Q_OBJECT

  public:
    struct ProjectSettings {
        QSize size = QSize(1024, 768);
        qint32 fps = 24;
        qint32 video_length = 5;
        QColor background_color = QColor("black");
    };

    explicit Renderer(QObject* parent = Q_NULLPTR);

    ProjectSettings projectSettings() const;
    void setProjectSettings(const ProjectSettings& new_project_settings);

    void setProjectSize(const QSize new_project_size);
    void setFPS(const qint32 new_fps);
    void setVideoLength(const qint32 new_video_length);
    void setBackgroundColor(const QColor& new_background_color);

    QImage createImage(const QList<QSharedPointer<ItemObserver>>& item_list, const qreal current_time) const;

  public slots:
    void render(const QList<QSharedPointer<ItemObserver>>& item_list, const QFileInfo& video_file);

  signals:
    void finishedRendering(const QFileInfo& video_file);

  private slots:
    void renderingProcessStarted(const QList<QSharedPointer<ItemObserver>>& item_list);
    void renderingProcessFinished(const QFileInfo& video_file, qint32 exitCode, QProcess::ExitStatus exitStatus);

  private:
    ProjectSettings m_project_settings;
    QMap<qint32, QSharedPointer<QProcess>> m_render_process_map;

    qint32 m_next_process_id = 0;
};

class RenderProcess : public QProcess {
    Q_OBJECT

  public:
    explicit RenderProcess(const qint32 id, QObject* parent = Q_NULLPTR)
        : QProcess { parent }
        , m_id(id)
    {
    }

    qint32 id() const { return m_id; }

  private:
    const qint32 m_id;
};

#endif // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_RENDERER_H_
