#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QStandardItemModel>

class MainWindow : public QObject
{

    Q_OBJECT

public:
    MainWindow(QQmlApplicationEngine *const engine);

public slots:
    void buttonClicked(const QVariantList &list);
    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void save(const QVariant &file, const QVariantList &scene_elements) const;
    void load(const QVariant &file) const;

    void addItem(QQuickItem *item);

private:
    QVariantList scene_elements;
    QList<QQuickItem *> m_item_list;

    QStandardItemModel m_itemModel;

    QQmlApplicationEngine *m_qml_engine;
};

#endif // MAINWINDOW_H
