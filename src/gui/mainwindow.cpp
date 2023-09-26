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
#include "logging.h"
#include "qqmlcontext.h"

QProcess *myProcess = new QProcess();

MainWindow::MainWindow(QQmlApplicationEngine *const engine)
    : m_qml_engine(engine)
{
    m_qml_engine->rootContext()->setContextProperty(QStringLiteral("item_model"), &m_itemModel);

    QStandardItem *headerItemLeft = new QStandardItem(tr("Name"));
    QStandardItem *headerItemRight = new QStandardItem(tr("Type"));
    m_itemModel.setHorizontalHeaderItem(0, headerItemLeft);
    m_itemModel.setHorizontalHeaderItem(1, headerItemRight);
}

void MainWindow::render()
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

    connect(myProcess, &QProcess::started, this, &MainWindow::processStarted);
    connect(myProcess, &QProcess::finished, this, &MainWindow::processFinished);
    connect(myProcess, &QProcess::readyRead, [=] {
        qCInfo(ffmpeg) << myProcess->readAllStandardOutput();
    });

    myProcess->setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);

    myProcess->start(program, arguments);
}

void MainWindow::processStarted()
{
    const int num_frames = 72;

    for (int frame = 0; frame < num_frames; ++frame) {
        //        auto alphaval = (double) frame / (double) num_frames;

        QImage image(600, 400, QImage::Format::Format_ARGB32);
        image.fill("white");
        QPainter painter(&image);

        for (const auto &item : m_item_list) {
            const auto qobj = qvariant_cast<AbstractItem *>(item->property("item"));

            painter.save();
            painter.translate(qobj->parentItem()->position());
            qobj->paint(&painter);
            painter.restore();
        }

        auto imageData = reinterpret_cast<const char *>(image.bits());
        myProcess->write(imageData, 600 * 400 * 4);
    }

    myProcess->closeWriteChannel();
    myProcess->waitForFinished();
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)

    qCDebug(rendering) << "Process finished with status: " << exitStatus;
}

void MainWindow::save(const QVariant &file) const
{
    QJsonObject save_json;
    int count = 0;
    QString element_prefix = "element_";

    for (const auto &item : m_item_list) {
        const auto qobj = qvariant_cast<AbstractItem *>(item->property("item"));

        const auto json_element = qobj->toJson();
        save_json[element_prefix + QString::number(count)] = json_element;
        count++;
    }

    QFile saveFile(file.toUrl().toLocalFile());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    saveFile.write(QJsonDocument(save_json).toJson());
}

void MainWindow::load(const QVariant &file)
{
    QFile loadFile(file.toUrl().toLocalFile());

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open save file. " << file.toUrl();
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

        QQmlComponent component(m_qml_engine, QUrl(element["item.file"].toString()));

        QObject *comp = component.createWithInitialProperties(element.toVariantMap());
        if (!comp) {
            qWarning("comp not found!");
        }
        QQuickItem *item = qobject_cast<QQuickItem *>(comp);
        if (!item) {
            qWarning("item  not found!");
        }
        item->setParentItem(qobject_cast<QQuickItem *>(oSelectArea));
        addItem(item);
    }
}

void MainWindow::addItem(QQuickItem *item)
{
    const auto qobj = item;
    m_item_list.append(qobj);

    const auto abstractItem = qvariant_cast<AbstractItem *>(item->property("item"));

    const auto itemName = abstractItem->name();

    int nameCounter = 0;
    for (const auto item : m_item_list) {
        const auto checkItemName = qvariant_cast<AbstractItem *>(item->property("item"))->name();

        if (checkItemName.startsWith(itemName)) {
            ++nameCounter;
        }
    }

    abstractItem->setName(itemName + "_" + QString::number(nameCounter));

    QStandardItem *stdItemName(new QStandardItem(abstractItem->name()));
    QStandardItem *stdItemType(new QStandardItem(abstractItem->metaObject()->className()));
    m_itemModel.appendRow(QList<QStandardItem *>{stdItemName, stdItemType});
}

void MainWindow::removeItem(QQuickItem *item)
{
    m_item_list.removeOne(item);

    auto itemNameList = m_itemModel.findItems("itemName");
    for (auto &item : itemNameList) {
        m_itemModel.removeRow(item->row());
    }

    item->deleteLater();
}

void MainWindow::removeRow(const int row)
{
    if (row == -1) {
        return;
    }

    m_itemModel.removeRow(row);

    if (m_item_list.size() <= row) {
        qCCritical(rendering) << "Row to delete is larger than item size!";
        return;
    }

    auto item = m_item_list.at(row);
    m_item_list.removeAt(row);

    item->deleteLater();
}

int MainWindow::getRowByItemName(QVariant name)
{
    const auto itemName = name.toString();

    const auto itemList = m_itemModel.findItems(itemName);

    if (itemList.isEmpty()) {
        qCCritical(rendering) << "Can't find clicked item. This shouldn't happen!";
        return -1;
    }

    return itemList.at(0)->row();
}

void MainWindow::clearAllItems()
{
    qDeleteAll(m_item_list.begin(), m_item_list.end());
    m_item_list.clear();

    m_itemModel.removeRows(0, m_itemModel.rowCount());
}
