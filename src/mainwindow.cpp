#include "mainwindow.h"

#include <iostream>

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QThread>

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairommconfig.h>

#include "circleitem.h"
#include "rectangle.h"

auto surface = Cairo::ImageSurface::create(Cairo::Format::FORMAT_ARGB32, 600, 400);
auto cr = Cairo::Context::create(surface);

unsigned char *start_data;

QProcess *myProcess = new QProcess();

//double rgba2rgb(double color, double alpha)
//{
//    return (1 - a)

//        return new Color((1 - alpha) * RGB_background.r + alpha * RGBA_color.r,
//                         (1 - alpha) * RGB_background.g + alpha * RGBA_color.g,
//                         (1 - alpha) * RGB_background.b + alpha * RGBA_color.b);
//}

MainWindow::MainWindow()
{
    cr->save(); // save the state of the context
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->paint();   // fill image with the color
    cr->restore(); // color is back to black now
    cr->save();

    start_data = surface->get_data();
}

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
        auto stride = Cairo::ImageSurface::format_stride_for_width(Cairo::Format::FORMAT_ARGB32,
                                                                   600);

        auto copy_data = new char[600 * 400 * 4];
        strncpy(copy_data, (char *) start_data, 600 * 400 * 4);
        auto surface_frame = Cairo::ImageSurface::create((unsigned char *) copy_data,
                                                         Cairo::Format::FORMAT_ARGB32,
                                                         600,
                                                         400,
                                                         stride);
        auto cr_frame = Cairo::Context::create(surface_frame);

        cr_frame->save();
        cr_frame->set_line_width(1.0);
        auto alphaval = (double) frame / (double) num_frames;
        cr_frame->set_source_rgba(1.0, 0.0, 0.0, alphaval);

        QImage image(start_data, 600, 400, QImage::Format::Format_ARGB32);
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
