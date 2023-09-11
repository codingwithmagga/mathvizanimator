#include "mainwindow.h"

#include <iostream>

#include <QDebug>
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

    qmlImageProvider_ = new QmlImageProvider();
    QImage image(start_data, 600, 400, QImage::Format_ARGB32);
    qmlImageProvider_->updateImage(image);
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

        for (const auto &element : scene_elements) {
            const auto qobj = qvariant_cast<QObject *>(element);
            const QString className = qobj->metaObject()->className();

            AbstractItem::ObjectContour rect_contour;

            if (className.compare("RectangleItem") == 0) {
                const auto obj = qvariant_cast<RectangleItem *>(element);
                rect_contour = obj->getObjectContour();
            } else if (className.compare("CircleItem") == 0) {
                const auto obj = qvariant_cast<CircleItem *>(element);
                rect_contour = obj->getObjectContour();
            } else {
                qWarning() << "unknwon item. continue";
                continue;
            }

            cr_frame->move_to(rect_contour.contour[0].start.x, rect_contour.contour[0].start.y);

            for (int i = 0; i < rect_contour.contour.length(); ++i) {
                auto x1 = rect_contour.contour[i].first_control_point.x;
                auto y1 = rect_contour.contour[i].first_control_point.y;
                auto x2 = rect_contour.contour[i].second_control_point.x;
                auto y2 = rect_contour.contour[i].second_control_point.y;
                auto x3 = rect_contour.contour[i].end.x;
                auto y3 = rect_contour.contour[i].end.y;

                cr_frame->curve_to(x1, y1, x2, y2, x3, y3);
            }

            cr_frame->close_path();
            cr_frame->stroke();
        }
        cr_frame->restore();
        auto data = reinterpret_cast<const char *>(surface_frame->get_data());

        myProcess->write(data, 600 * 400 * 4);

        delete[] copy_data;
    }

    myProcess->closeWriteChannel();
    myProcess->waitForFinished();
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with status: " << exitStatus;
}
