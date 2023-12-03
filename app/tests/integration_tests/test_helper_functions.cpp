/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "test_helper_functions.h"

#include <QAbstractEventDispatcher>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>

TestHelperFunctions::TestHelperFunctions(
    const QSharedPointer<QQmlApplicationEngine> engine)
    : m_engine(engine) {
  auto root_objects = m_engine->rootObjects();
  QVERIFY2(!root_objects.isEmpty(), "No QML root objects created!");
  /* TODO (codingwithmagga)
   * In general it is not a good idea to use QVERIFY (or other QTest macros) out
   * of test functions, because they return immediately when an error occurs.
   * This should be rewritten later on.
   */

  QObject* root = root_objects.first();
  m_quick_window = qobject_cast<QQuickWindow*>(root);
  QVERIFY2(m_quick_window, "Main QQuickWindow not found.");

  m_draggable_item_list_view =
      root_objects.first()->findChild<QObject*>("MVADraggableItemListView");
  m_project_items_table_view =
      root_objects.first()->findChild<QObject*>("MVAProjectItemsTableView");
  m_creation_area =
      root_objects.first()->findChild<QQuickItem*>("MVACreationArea");

  QVERIFY2(m_draggable_item_list_view && m_project_items_table_view &&
               m_creation_area,
           "Important qml objects are missing!");
}

void TestHelperFunctions::dragAndDropCurrentItem(const QPoint& end_pos) {
  auto current_item = qobject_cast<QQuickItem*>(
      m_draggable_item_list_view->property("currentItem").value<QObject*>());

  const QPoint start_pos = current_item->mapToScene(QPointF(0, 0)).toPoint();
  const QPoint end_global_pos =
      m_creation_area->mapToScene(QPointF(0.0, 0.0)).toPoint() + end_pos;

  dragAndDropItem(start_pos, end_global_pos);
}

void TestHelperFunctions::dragAndDropItem(const QPoint& start_pos,
                                          const QPoint& end_pos) {
  const QPoint minimum_drag_distance(20, 20);

  QTest::mousePress(m_quick_window, Qt::LeftButton, Qt::NoModifier, start_pos);
  QTest::mouseMove(m_quick_window, start_pos + minimum_drag_distance);
  QTest::mouseRelease(m_quick_window, Qt::LeftButton, Qt::NoModifier, end_pos);
}

void TestHelperFunctions::clickItem(QQuickItem* quick_item) {
  auto oPoint = quick_item->mapToScene(QPoint(0, 0)).toPoint();

  oPoint.rx() += quick_item->width() / 2;
  oPoint.ry() += quick_item->height() / 2;

  QTest::mouseClick(m_quick_window, Qt::LeftButton, Qt::NoModifier, oPoint);
}

qint32 TestHelperFunctions::numCreationAreaItems() const {
  return m_creation_area->childItems().size();
}

qint32 TestHelperFunctions::numProjectTableViewItems() const {
  return m_project_items_table_view->property("rows").toInt();
}
