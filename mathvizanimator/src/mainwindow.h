#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cairommconfig.h>

#include "qmlimageprovider.h"

class MainWindow : public QObject
{

    Q_OBJECT

public:
    MainWindow();

    QmlImageProvider *getImageProvider() const { return qmlImageProvider_; };

public slots:
    void buttonClicked(const QVariantList &list);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QmlImageProvider *qmlImageProvider_;

    QVariantList scene_elements;
};

#endif // MAINWINDOW_H
