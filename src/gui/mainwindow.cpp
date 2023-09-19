#include "mainwindow.h"

#include <iostream>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QPainter>
#include <QPainterPath>
#include <QProcess>
#include <QThread>

#include "abstractitem.h"
#include "rectangle.h"

QProcess *myProcess = new QProcess();

MainWindow::MainWindow(QQmlApplicationEngine *const engine)
    : m_qml_engine(engine)
{}

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

            painter.save();
            painter.translate(qobj->parentItem()->position());
            qobj->paint(&painter);
            painter.restore();

            //            for (int i = 0; i < qobj->metaObject()->propertyCount(); ++i) {
            //                qDebug() << qobj->metaObject()->property(i).name()
            //                         << qobj->metaObject()->property(i).read(qobj);
            //            }
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

void MainWindow::save(const QVariantList &scene_elements) const
{
    QJsonObject save_json;
    int count = 0;
    QString element_prefix = "element_";

    for (const auto &element : scene_elements) {
        const auto qobj = qvariant_cast<AbstractItem *>(element);

        const auto json_element = qobj->toJson();
        save_json[element_prefix + QString::number(count)] = json_element;
        count++;
    }

    QFile saveFile("save.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    saveFile.write(QJsonDocument(save_json).toJson());
}

void MainWindow::load() const
{
    QFile loadFile("save.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QObject *oRootObject = dynamic_cast<QObject *>(m_qml_engine->rootObjects()[0]);
    QObject *oSelectArea = oRootObject->findChild<QObject *>("selectArea");

    if (!oRootObject) {
        qWarning("oRootObject not found!");
    }
    if (!oSelectArea) {
        qWarning("oSelectArea not found!");
    }

    QJsonObject json = loadDoc.object();
    foreach (const QString &elementKey, json.keys()) {
        auto element = json[elementKey].toObject();

        QQmlComponent component(m_qml_engine, QUrl(element["file"].toString()));

        QObject *comp = component.createWithInitialProperties(element.toVariantMap());
        if (!comp) {
            qWarning("comp not found!");
        }
        QQuickItem *item = qobject_cast<QQuickItem *>(comp);
        if (!item) {
            qWarning("item  not found!");
        }
        item->setParentItem(qobject_cast<QQuickItem *>(oSelectArea));
    }
}
