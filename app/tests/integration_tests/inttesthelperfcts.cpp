#include "inttesthelperfcts.h"

#include <QAbstractEventDispatcher>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QTest>

IntTestHelperFcts::IntTestHelperFcts(
    const QSharedPointer<QQmlApplicationEngine> engine)
    : m_engine(engine) {
  auto root_objects = m_engine->rootObjects();
  QVERIFY2(!root_objects.isEmpty(), "No QML root objects created!");
  /* TODO (codingwithmagga)
   * In general it is not a good idea to use QVERIFY (or other QTest macros) out
   * of test fcts, because they return immediately when an error occurs.
   * This should be rewritten later on.
   */

  QObject* root = root_objects.first();
  m_quick_window = qobject_cast<QQuickWindow*>(root);
  QVERIFY2(m_quick_window, "Main QQuickWindow not found.");

  m_dragable_item_list_view =
      root_objects.first()->findChild<QObject*>("MVADragableItemListView");
  m_project_items_table_view =
      root_objects.first()->findChild<QObject*>("MVAProjectItemsTableView");
  m_creation_area =
      root_objects.first()->findChild<QQuickItem*>("MVACreationArea");

  QVERIFY2(m_dragable_item_list_view && m_project_items_table_view &&
               m_creation_area,
           "Important qml objects are missing!");
}

void IntTestHelperFcts::dragAndDropCurrentItem(const QPoint& end_pos) {
  auto current_item = qobject_cast<QQuickItem*>(
      m_dragable_item_list_view->property("currentItem").value<QObject*>());

  const QPoint start_pos = current_item->mapToGlobal(0, 0).toPoint();
  const QPoint end_global_pos =
      m_creation_area->mapToScene(QPointF(0.0, 0.0)).toPoint() + end_pos;

  dragAndDropItem(start_pos, end_global_pos);
}

void IntTestHelperFcts::dragAndDropItem(const QPoint& start_pos,
                                        const QPoint& end_pos) {
  const QPoint minimum_drag_distance(20, 20);

  QTest::mousePress(m_quick_window, Qt::LeftButton, Qt::NoModifier, start_pos);
  QTest::mouseMove(m_quick_window, start_pos + minimum_drag_distance);
  QTest::mouseRelease(m_quick_window, Qt::LeftButton, Qt::NoModifier, end_pos);
}

void IntTestHelperFcts::clickItem(QQuickItem* quick_item) {
  auto oPoint = quick_item->mapToScene(QPoint(0, 0)).toPoint();

  oPoint.rx() += quick_item->width() / 2;
  oPoint.ry() += quick_item->height() / 2;

  QTest::mouseClick(m_quick_window, Qt::LeftButton, Qt::NoModifier, oPoint);
}

qint32 IntTestHelperFcts::numCreationAreaItems() const {
  return m_creation_area->childItems().size();
}

qint32 IntTestHelperFcts::numProjectTableViewItems() const {
  return m_project_items_table_view->property("rows").toInt();
}

void IntTestHelperFcts::processEvents(const qint32 waiting_time) {
  while (QThread::currentThread()->eventDispatcher()->processEvents(
      QEventLoop::AllEvents)) {
    QTest::qWait(waiting_time);
  }
}
