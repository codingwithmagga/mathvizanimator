#ifndef RENDERER_H
#define RENDERER_H

#include <QFileInfo>
#include <QObject>
#include <QProcess>

#include "abstractitem.h"
#include "workflow_global.h"

class WORKFLOW_EXPORT Renderer : public QObject
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = nullptr);

public slots:
    void render(const QList<AbstractItem *> &item_list);

signals:
    void finishedRendering(const QFileInfo &video_file);

private slots:
    void renderingProcessStarted(const QList<AbstractItem *> &item_list);
    void renderingProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess renderProcess;
};

#endif // RENDERER_H
