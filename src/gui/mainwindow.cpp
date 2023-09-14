#include "mainwindow.h"

#include <iostream>

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QThread>

#include "abstractitem.h"

QProcess *myProcess = new QProcess();

MainWindow::MainWindow() {}

void MainWindow::buttonClicked(const QVariantList &list)
{
    scene_elements = list;

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

    connect(myProcess, &QProcess::started, this, &MainWindow::processStarted);
    connect(myProcess, &QProcess::finished, this, &MainWindow::processFinished);

    myProcess->setProcessChannelMode(QProcess::ProcessChannelMode::ForwardedChannels);

    myProcess->start(program, arguments);
}

void MainWindow::processStarted()
{
    const int num_frames = 72;

    for (int frame = 0; frame < num_frames; ++frame) {
        auto alphaval = (double) frame / (double) num_frames;

        QImage image(600, 400, QImage::Format::Format_ARGB32);
        image.fill("white");
        QPainter painter(&image);

        for (const auto &element : scene_elements) {
            const auto qobj = qvariant_cast<AbstractItem *>(element);

            painter.translate(qobj->parentItem()->position());
            qobj->paint(&painter);
            painter.resetTransform();
        }

        auto imageData = reinterpret_cast<const char *>(image.bits());
        myProcess->write(imageData, 600 * 400 * 4);
    }

    myProcess->closeWriteChannel();
    myProcess->waitForFinished();
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with status: " << exitStatus;
}
