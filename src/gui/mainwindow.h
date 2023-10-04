#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QStandardItemModel>

#include "savefilehandler.h"

class MainWindowHandler : public QObject
{

    Q_OBJECT

public:
    MainWindowHandler(QQmlApplicationEngine *const engine);

    void init();

public slots:

    void render();

    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void save(const QVariant &file);
    void load(const QVariant &file);

    void addItem(QQuickItem *item);
    void removeItem(QQuickItem *item);
    void removeRow(const int row);

    int getRowByItemName(QVariant name);

    void clearAllItems();

private:
    QList<QQuickItem *> m_item_list;
    QStandardItemModel m_itemModel;

    QQmlApplicationEngine *m_qml_engine;
    QObject *m_qml_creation_area;

    SaveFileHandler m_savefile_handler;
};

#endif // MAINWINDOW_H
