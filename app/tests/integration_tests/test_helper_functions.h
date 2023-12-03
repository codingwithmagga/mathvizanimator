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

#ifndef TEST_HELPER_FUNCTIONS_H
#define TEST_HELPER_FUNCTIONS_H

#include <QSharedPointer>

class QQuickItem;
class QQuickWindow;
class QQmlApplicationEngine;

class TestHelperFunctions {
 public:
  explicit TestHelperFunctions(
      const QSharedPointer<QQmlApplicationEngine> engine);

  void dragAndDropCurrentItem(const QPoint &end_pos);
  void dragAndDropItem(const QPoint &start_pos, const QPoint &end_pos);

  void clickItem(QQuickItem *quick_item);

  QQuickWindow *rootWindow() const { return m_quick_window; }
  QObject *draggableItemListView() const { return m_draggable_item_list_view; }
  QObject *projectItemsTableView() const { return m_project_items_table_view; }
  QQuickItem *creationArea() const { return m_creation_area; }

  qint32 numCreationAreaItems() const;
  qint32 numProjectTableViewItems() const;

  static void processEvents(const qint32 waiting_time = 1000);

 private:
  QSharedPointer<QQmlApplicationEngine> m_engine;

  QQuickWindow *m_quick_window = Q_NULLPTR;
  QObject *m_draggable_item_list_view = Q_NULLPTR;
  QObject *m_project_items_table_view = Q_NULLPTR;
  QQuickItem *m_creation_area = Q_NULLPTR;
};

#endif  // TEST_HELPER_FUNCTIONS_H
