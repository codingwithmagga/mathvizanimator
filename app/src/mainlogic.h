#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QLoggingCategory>
#include <QObject>
#include <QQmlApplicationEngine>

#include "itemhandler.h"
#include "mainwindowhandler.h"
#include "renderer.h"
#include "savefilehandler.h"

Q_DECLARE_LOGGING_CATEGORY(mainlogic)

class MainLogic : public QObject {
  Q_OBJECT

 public:
  explicit MainLogic(QObject *parent = Q_NULLPTR);

  void init();
  void initEngine(QQmlApplicationEngine *const engine);

 private slots:
  void createSnapshot();
  void renderVideo();

  void saveProject(const QFileInfo &savefileinfo);
  void loadProject(const QFileInfo &loadfileinfo);

  void addItem(QQuickItem *quick_item);
  void removeItem(QQuickItem *quick_item);

  void projectWidthChanged(const qint32 new_project_width);
  void projectHeightChanged(const qint32 new_project_height);

 private:
  QList<AbstractItem *> getAbstractItemList();

  QQmlApplicationEngine *m_qml_engine;
  QObject *m_qml_creation_area;

  MainWindowHandler m_mainwindowhandler;

  SaveFileHandler m_savefilehandler;
  Renderer m_renderer;
  ItemHandler m_itemhandler;
};

#endif  // MAINLOGIC_H
