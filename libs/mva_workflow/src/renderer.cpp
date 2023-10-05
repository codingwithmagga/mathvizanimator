#include "renderer.h"

#include <QImage>
#include <QPainter>

#include "logging.h"

Renderer::Renderer(QObject *parent)
    : QObject{parent}
{
    
}

void Renderer::render(const QList<AbstractItem *> &item_list)
{
    QString program = "/usr/bin/ffmpeg";
    QStringList arguments;
    arguments << "-y"
              << "-f"
              << "rawvideo"
              << "-video_size"
              << "600x400"
              << "-pix_fmt"
              << "bgra"
              << "-r"
              << "24"
              << "-i"
              << "-"
              << "-an"
              << "-vcodec"
              << "libx264"
              << "render_test.mp4";

    connect(&renderProcess, &QProcess::started, [item_list, this] {
        renderingProcessStarted(item_list);
    });
    connect(&renderProcess, &QProcess::finished, this, &Renderer::renderingProcessFinished);
    connect(&renderProcess, &QProcess::readyRead, [this] {
        qCInfo(renderer) << renderProcess.readAllStandardOutput();
    });

    renderProcess.setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

    renderProcess.start(program, arguments);
}

void Renderer::renderingProcessStarted(const QList<AbstractItem *> &item_list)
{
    const int num_frames = 72;

    for (int frame = 0; frame < num_frames; ++frame) {
        QImage image(600, 400, QImage::Format::Format_ARGB32);
        image.fill("white");
        QPainter painter(&image);

        for (const auto &item : item_list) {
            painter.save();
            painter.translate(item->parentItem()->position());
            item->paint(&painter);
            painter.restore();
        }

        auto imageData = reinterpret_cast<const char *>(image.bits());
        renderProcess.write(imageData, 600 * 400 * 4);
    }

    renderProcess.closeWriteChannel();
    renderProcess.waitForFinished();
}

void Renderer::renderingProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)

    qCDebug(renderer) << "Process finished with status: " << exitStatus;
    emit finishedRendering(QFileInfo("render_test.mp4"));
}
