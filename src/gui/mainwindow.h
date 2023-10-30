#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QStandardItemModel>

#include "itemhandler.h"
#include "renderer.h"
#include "savefilehandler.h"

class MainWindowHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 pixel_width READ pixelWidth WRITE setPixelWidth NOTIFY pixelWidthChanged)
    Q_PROPERTY(qint32 pixel_height READ pixelHeight WRITE setPixelHeight NOTIFY pixelHeightChanged)
    Q_PROPERTY(qint32 fps READ fps WRITE setFPS NOTIFY fpsChanged)

public:
    MainWindowHandler();

    void init();
    void initEngine(QQmlApplicationEngine *const engine);

public slots:

    void render();

    void save(const QVariant &file);
    void load(const QVariant &file);

    void addItem(QQuickItem *quick_item);
    void removeItem(QQuickItem *quick_item);
    void removeRow(const int row);

    int getRowByItemName(QVariant name);

    void updateProjectSettings(QVariantList newProjectSettings);

    void clearAllItems();

    void currentRowChanged(const int row);

    qint32 pixelWidth() const;
    qint32 pixelHeight() const;
    qint32 fps() const;

    void setPixelWidth(qint32 new_pixel_width);
    void setPixelHeight(qint32 new_pixel_height);
    void setFPS(qint32 new_fps);

    void openSVGFolder() const;

signals:
    void pixelWidthChanged(const qint32 new_pixel_width);
    void pixelHeightChanged(const qint32 new_pixel_height);
    void fpsChanged(const qint32 new_fps);

private:
    QQmlApplicationEngine *m_qml_engine;
    QObject *m_qml_creation_area;

    SaveFileHandler m_savefile_handler;
    Renderer m_renderer;
    ItemHandler m_itemhandler;
};

#endif // MAINWINDOW_H
