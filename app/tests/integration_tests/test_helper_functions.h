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

#ifndef APP_TESTS_INTEGRATION_TESTS_TEST_HELPER_FUNCTIONS_H_
#define APP_TESTS_INTEGRATION_TESTS_TEST_HELPER_FUNCTIONS_H_

#include <QQuickWindow>
#include <QSharedPointer>

#include "item_observer.h"

class QQuickItem;
class QQuickWindow;
class QStandardItemModel;
class QQmlApplicationEngine;
class MainWindowHandler;

class TestHelperFunctions {
  public:
    explicit TestHelperFunctions(const QSharedPointer<QQmlApplicationEngine> engine);

    void dragAndDropCurrentItem(const QPoint& end_pos);
    void dragAndDropItem(const QPoint& start_pos, const QPoint& end_pos);

    void addAnimationToItem(const qint32 item_number, const qreal start_time, const qreal duration);

    void clickItem(QQuickItem* quick_item, Qt::MouseButton mouse_button = Qt::MouseButton::LeftButton);
    void moveItem(QQuickItem* quick_item, const QPoint& move_dist);

    void changeTime(const qreal time);

    QQuickWindow* rootWindow() const { return m_quick_window; }
    QObject* draggableItemListView() const { return m_draggable_item_list_view; }
    QObject* projectItemsTableView() const { return m_project_items_table_view; }
    QQuickItem* creationArea() const { return m_creation_area; }
    MainWindowHandler* mainWindowHandler() const { return m_main_window_handler; }

    QSharedPointer<ItemObserver> getItemObserver(const qint32 item_number) const;
    QQuickItem* getQuickItem(const qint32 item_number) const;

    QQuickItem* getAnimationItem(const qint32 animation_number) const;

    qint32 numCreationAreaItems() const;
    qint32 numProjectTableViewItems() const;
    qint32 numPropertyTableViewItems() const;

    void changePropertyValue(const qint32 item_number, const QString& property_name, const QString& property_value);
    QVariant getPropertyValue(const QString& property_name) const;

    bool compareNumItems(const qint32 num_items);
    bool compareNumAnimations(const QString item_name, const qint32 num_animations);

    bool renderVideo(const QString& render_file) const;
    QImage extractImage(const QString& video_file) const;

    void saveFile() const;
    bool saveFileAs(const QString& full_file_path) const;
    bool loadFile(const QString& full_file_path) const;

    QString copyFileToTestDir(const QFile& file, const QString& new_file_name) const;

    template <typename T> T getChild(const QString& name) const
    {
        auto child = m_quick_window->findChild<T>(name);
        if (!child) {
            qFatal() << "Couldn't find child with name" << name;
        }
        return child;
    }

    static QString absoluteFilePath(const QString file_name);

  private:
    QPoint itemCenter(QQuickItem* item) const;

    QSharedPointer<QQmlApplicationEngine> m_engine;

    QQuickWindow* m_quick_window = Q_NULLPTR;
    QObject* m_draggable_item_list_view = Q_NULLPTR;
    QObject* m_project_items_table_view = Q_NULLPTR;
    QObject* m_property_table_view = Q_NULLPTR;
    QObject* m_animations_table_view = Q_NULLPTR;
    QStandardItemModel* m_project_items_model = Q_NULLPTR;
    QStandardItemModel* m_property_model = Q_NULLPTR;
    QStandardItemModel* m_animations_model = Q_NULLPTR;
    QQuickItem* m_creation_area = Q_NULLPTR;
    QQuickItem* m_time_slider = Q_NULLPTR;

    MainWindowHandler* m_main_window_handler = Q_NULLPTR;
};

#endif // APP_TESTS_INTEGRATION_TESTS_TEST_HELPER_FUNCTIONS_H_
