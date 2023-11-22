#include "mainlogic.h"

#include <QQmlContext>
#include <QStandardPaths>

Q_LOGGING_CATEGORY(mainlogic, "cwa.mva.mainlogic")

MainLogic::MainLogic(QObject *parent) : QObject{parent} {
  m_savefilehandler.setSaveDir(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

  connect(&m_mainwindowhandler, &MainWindowHandler::snapshotRequested, this,
          &MainLogic::createSnapshot);
  connect(&m_mainwindowhandler, &MainWindowHandler::renderingRequested, this,
          &MainLogic::renderVideo);
  connect(&m_mainwindowhandler, &MainWindowHandler::saveProjectRequested, this,
          &MainLogic::saveProject);
  connect(&m_mainwindowhandler, &MainWindowHandler::loadProjectRequested, this,
          &MainLogic::loadProject);

  connect(&m_mainwindowhandler, &MainWindowHandler::removeCurrentItemRequested,
          &m_itemhandler, &ItemHandler::removeCurrentItem);
  connect(&m_mainwindowhandler, &MainWindowHandler::itemClicked, &m_itemhandler,
          &ItemHandler::setCurrentItem);
  connect(&m_mainwindowhandler, &MainWindowHandler::newProjectRequested,
          &m_itemhandler, &ItemHandler::clear);

  connect(&m_mainwindowhandler, &MainWindowHandler::pixelWidthChanged, this,
          &MainLogic::projectWidthChanged);
  connect(&m_mainwindowhandler, &MainWindowHandler::pixelHeightChanged, this,
          &MainLogic::projectHeightChanged);
  connect(&m_mainwindowhandler, &MainWindowHandler::fpsChanged, &m_renderer,
          &Renderer::setFPS);
  connect(&m_mainwindowhandler, &MainWindowHandler::videoLengthChanged,
          &m_renderer, &Renderer::setVideoLength);

  const auto default_project_settings = m_renderer.projectSettings();
  QList<qint32> conv_project_settings{
      default_project_settings.width, default_project_settings.height,
      default_project_settings.fps, default_project_settings.video_length};
  m_mainwindowhandler.updateProjectSettings(conv_project_settings);
}

void MainLogic::initEngine(QQmlApplicationEngine *const engine) {
  m_qml_engine = engine;

  m_qml_engine->rootContext()->setContextProperty(QStringLiteral("main_window"),
                                                  &m_mainwindowhandler);
  m_qml_engine->rootContext()->setContextProperty(QStringLiteral("item_model"),
                                                  m_itemhandler.model());
  m_qml_engine->rootContext()->setContextProperty(
      QStringLiteral("item_selection_model"), m_itemhandler.selectionmodel());
  m_qml_engine->rootContext()->setContextProperty(
      QStringLiteral("property_model"), m_itemhandler.propertyModel());
}

void MainLogic::createSnapshot() {
  const auto item_list = getAbstractItemList();
  const auto snapshot = m_renderer.createImage(item_list);

  snapshot.save("snapshot.png");
}

void MainLogic::renderVideo() {
  const auto item_list = getAbstractItemList();
  m_renderer.render(item_list);
}

void MainLogic::saveProject(const QFileInfo &savefileinfo) {
  QJsonObject save_json;
  qint32 count = 0;
  QString element_prefix = "element_";
  const auto item_list = m_itemhandler.items();

  for (const auto &item : item_list) {
    const auto abstract_item =
        qvariant_cast<AbstractItem *>(item->property("item"));

    const auto json_element = abstract_item->toJson();
    save_json[element_prefix + QString::number(count)] = json_element;
    count++;
  }

  m_savefilehandler.setSaveDir(savefileinfo.absoluteDir());
  m_savefilehandler.saveJSON(savefileinfo.fileName(), save_json);
}

void MainLogic::loadProject(const QFileInfo &loadfileinfo) {
  QJsonDocument loadDoc = m_savefilehandler.loadJSON(loadfileinfo);

  QJsonObject json = loadDoc.object();
  for (const QString &elementKey : json.keys()) {
    auto element = json[elementKey].toObject();

    QQmlComponent component(m_qml_engine,
                            QUrl(element["item.file"].toString()));

    auto elementProperties = element.toVariantMap();

    // "file" is a read-only property which will be set when the item is
    // created. It is not possible (or necessary) to set it in this context.
    elementProperties.remove("item.file");

    elementProperties.insert("parent",
                             QVariant::fromValue(m_qml_creation_area));
    QObject *comp = component.createWithInitialProperties(elementProperties);
    if (!comp) {
      qCWarning(mainlogic) << "component not found!";
    }
    QQuickItem *item = qobject_cast<QQuickItem *>(comp);
    if (!item) {
      qCWarning(mainlogic) << "item not found!";
    }

    addItem(item);
  }
}

void MainLogic::init() {
  const auto rootObjects = m_qml_engine->rootObjects();

  if (rootObjects.isEmpty()) {
    qCCritical(mainlogic)
        << "No root objects found in current qml engine. Init aborted.";
    return;
  }

  m_qml_creation_area =
      rootObjects.first()->findChild<QObject *>("creationArea");

  if (!m_qml_creation_area) {
    qCCritical(mainlogic)
        << "Can't find creation area for videos. Init aborted.";
    return;
  }

  QObject::connect(m_qml_creation_area, SIGNAL(itemAdded(QQuickItem *)), this,
                   SLOT(addItem(QQuickItem *)));
}

QList<AbstractItem *> MainLogic::getAbstractItemList() {
  QList<AbstractItem *> abstractitem_list;

  const auto item_list = m_itemhandler.items();
  for (const auto &item : item_list) {
    abstractitem_list.push_back(
        qvariant_cast<AbstractItem *>(item->property("item")));
  }

  return abstractitem_list;
}

void MainLogic::addItem(QQuickItem *quick_item) {
  m_itemhandler.addItem(quick_item);
}

void MainLogic::removeItem(QQuickItem *quick_item) {
  m_itemhandler.removeItem(quick_item);
}

void MainLogic::projectWidthChanged(const qint32 new_project_width) {
  if (m_renderer.projectSettings().width == new_project_width) {
    return;
  }

  const qreal ratio =
      new_project_width / qreal(m_renderer.projectSettings().width);
  m_itemhandler.scaleItemsX(ratio);
  m_itemhandler.scaleItemsWidth(ratio);

  m_renderer.setWidth(new_project_width);
}

void MainLogic::projectHeightChanged(const qint32 new_project_height) {
  if (m_renderer.projectSettings().height == new_project_height) {
    return;
  }

  const qreal ratio =
      new_project_height / qreal(m_renderer.projectSettings().height);
  m_itemhandler.scaleItemsY(ratio);
  m_itemhandler.scaleItemsHeight(ratio);

  m_renderer.setHeight(new_project_height);
}
