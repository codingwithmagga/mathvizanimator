#ifndef INTTESTHELPERFCTS_H
#define INTTESTHELPERFCTS_H

#include <QSharedPointer>

class QQuickItem;
class QQuickWindow;
class QQmlApplicationEngine;

class IntTestHelperFcts {
 public:
  IntTestHelperFcts(const QSharedPointer<QQmlApplicationEngine> engine);

  void dragAndDropCurrentItem(const QPoint &end_pos);
  void dragAndDropItem(const QPoint &start_pos, const QPoint &end_pos);

  void clickItem(QQuickItem *quick_item);

  QQuickWindow *rootWindow() const { return m_quick_window; }
  QObject *dragAbleItemListView() const { return m_dragable_item_list_view; }
  QObject *projectItemsTableView() const { return m_project_items_table_view; }
  QQuickItem *creationArea() const { return m_creation_area; }

  qint32 numCreationAreaItems() const;
  qint32 numProjectTableViewItems() const;

  static void processEvents(const qint32 waiting_time = 50);

 private:
  QSharedPointer<QQmlApplicationEngine> m_engine;

  QQuickWindow *m_quick_window = Q_NULLPTR;
  QObject *m_dragable_item_list_view = Q_NULLPTR;
  QObject *m_project_items_table_view = Q_NULLPTR;
  QQuickItem *m_creation_area = Q_NULLPTR;
};

#endif  // INTTESTHELPERFCTS_H
