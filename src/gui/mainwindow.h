#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>

class MainWindow : public QObject
{

    Q_OBJECT

public:
    MainWindow(QQmlApplicationEngine *const engine);

public slots:
    void buttonClicked(const QVariantList &list);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void save(const QVariantList &scene_elements) const;
    void load() const;

private:
    QVariantList scene_elements;

    QQmlApplicationEngine *m_qml_engine;
};

#endif // MAINWINDOW_H
