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
    void render();

    void processStarted();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void save(const QVariant &file) const;
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
};

#endif // MAINWINDOW_H
