#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QStandardItemModel>

#include "renderer.h"
#include "savefilehandler.h"

class MainWindowHandler : public QObject
{

    Q_OBJECT

public:
    MainWindowHandler(QQmlApplicationEngine *const engine);

    void init();

public slots:

    void render();

    void save(const QVariant &file);
    void load(const QVariant &file);

    void addItem(QQuickItem *quick_item);
    void removeItem(QQuickItem *quick_item);
    void removeRow(const int row);

    int getRowByItemName(QVariant name);

    void clearAllItems();

private:
    QList<QQuickItem *> m_item_list;
    QStandardItemModel m_itemModel;

    QQmlApplicationEngine *m_qml_engine;
    QObject *m_qml_creation_area;

    SaveFileHandler m_savefile_handler;
    Renderer m_renderer;
};

#endif // MAINWINDOW_H
