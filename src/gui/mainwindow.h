#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>


class MainWindow : public QObject
{

    Q_OBJECT

public:
    MainWindow();

public slots:
    void buttonClicked(const QVariantList &list);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QVariantList scene_elements;
};

#endif // MAINWINDOW_H
