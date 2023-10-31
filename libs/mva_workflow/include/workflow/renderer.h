#ifndef RENDERER_H
#define RENDERER_H

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
    void renderingProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess m_render_process;
    ProjectSettings m_project_settings;
};

#endif // RENDERER_H
