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
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSignalSpy>
#include <QStandardItemModel>
#include <QTest> // should be removed, see comment in constructor

#include "itemhandler.h"
#include "mainwindowhandler.h"

TestHelperFunctions::TestHelperFunctions(const QSharedPointer<QQmlApplicationEngine> engine)
    : m_engine(engine)
{
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

    m_draggable_item_list_view = root_objects.first()->findChild<QObject*>("MVADraggableItemListView");
    m_project_items_table_view = root_objects.first()->findChild<QObject*>("MVAProjectItemsTableView");
    m_property_table_view = root_objects.first()->findChild<QObject*>("MVAPropertyTable");
    m_animations_table_view = root_objects.first()->findChild<QObject*>("MVAAnimationTable");
    m_creation_area = root_objects.first()->findChild<QQuickItem*>("MVACreationArea");
    m_time_slider = root_objects.first()->findChild<QQuickItem*>("MVATimeSlider");
    m_main_window_handler = m_engine->rootContext()->contextProperty("main_window").value<MainWindowHandler*>();

    auto items_model = m_project_items_table_view->property("model");
    m_project_items_model = qvariant_cast<QStandardItemModel*>(items_model);
    auto property_model = m_property_table_view->property("model");
    m_property_model = qvariant_cast<QStandardItemModel*>(property_model);
    auto animation_model = m_animations_table_view->property("model");
    m_animations_model = qvariant_cast<QStandardItemModel*>(animation_model);

    QVERIFY2(m_draggable_item_list_view && m_project_items_table_view && m_property_table_view && m_creation_area
            && m_project_items_model,
        "Important qml objects are missing!");
}

void TestHelperFunctions::dragAndDropCurrentItem(const QPoint& end_pos)
{
    auto current_item
        = qobject_cast<QQuickItem*>(m_draggable_item_list_view->property("currentItem").value<QObject*>());

    const QPoint start_pos = current_item->mapToScene(QPointF(0, 0)).toPoint();
    const QPoint end_global_pos = m_creation_area->mapToScene(QPointF(0.0, 0.0)).toPoint() + end_pos;

    dragAndDropItem(start_pos, end_global_pos);
}

void TestHelperFunctions::dragAndDropItem(const QPoint& start_pos, const QPoint& end_pos)
{
    const QPoint minimum_drag_distance(20, 20);

    QTest::mousePress(m_quick_window, Qt::LeftButton, Qt::NoModifier, start_pos, 20);
    QTest::mouseMove(m_quick_window, start_pos + minimum_drag_distance, 20);
    QTest::mouseRelease(m_quick_window, Qt::LeftButton, Qt::NoModifier, end_pos, 20);
}

void TestHelperFunctions::clickItem(QQuickItem* quick_item, Qt::MouseButton mouse_button)
{
    QTest::mouseClick(m_quick_window, mouse_button, Qt::NoModifier, itemCenter(quick_item));
}

void TestHelperFunctions::moveItem(QQuickItem* quick_item, const QPoint& move_dist)
{
    dragAndDropItem(itemCenter(quick_item), itemCenter(quick_item) + move_dist);
}

void TestHelperFunctions::changeTime(const qreal time) { m_time_slider->setProperty("value", time); }

QSharedPointer<ItemObserver> TestHelperFunctions::getItemObserver(const qint32 item_number) const
{
    const auto item = dynamic_cast<ItemModelItem*>(m_project_items_model->item(item_number));

    return item->itemObserver();
}

QQuickItem* TestHelperFunctions::getQuickItem(const qint32 item_number) const
{
    return getItemObserver(item_number)->item();
}

QQuickItem* TestHelperFunctions::getAnimationItem(const qint32 animation_number) const
{
    const auto rowLoaded = QTest::qWaitFor([&]() {
        bool rowLoaded = false;
        QMetaObject::invokeMethod(
            m_animations_table_view, "isRowLoaded", Q_RETURN_ARG(bool, rowLoaded), Q_ARG(int, animation_number));

        return rowLoaded;
    });

    if (!rowLoaded) {
        return nullptr;
    }

    QQuickItem* item = nullptr;
    QMetaObject::invokeMethod(m_animations_table_view, "itemAtCell", Q_RETURN_ARG(QQuickItem*, item),
        Q_ARG(QPoint, QPoint(animation_number, 0)));

    return item;
}

qint32 TestHelperFunctions::numCreationAreaItems() const { return m_creation_area->childItems().size(); }

qint32 TestHelperFunctions::numProjectTableViewItems() const
{
    return m_project_items_table_view->property("rows").toInt();
}

qint32 TestHelperFunctions::numPropertyTableViewItems() const
{
    return m_property_table_view->property("rows").toInt();
}

void TestHelperFunctions::changePropertyValue(
    const qint32 item_number, const QString& property_name, const QString& property_value)
{
    clickItem(getQuickItem(item_number));
    for (int row = 0; row < m_property_model->rowCount(); ++row) {
        const auto model_property_name = m_property_model->item(row);
        if (model_property_name->text() == property_name) {
            m_property_model->item(row, 1)->setText(property_value);
        }
    }
}

QVariant TestHelperFunctions::getPropertyValue(const QString& property_name) const
{
    QVariant property_value;
    for (int row = 0; row < m_property_model->rowCount(); ++row) {
        const auto model_property_name = m_property_model->item(row);
        if (model_property_name->text() == property_name) {
            property_value = m_property_model->item(row, 1)->text();
        }
    }

    return property_value;
}

bool TestHelperFunctions::compareNumItems(const qint32 num_items)
{
    // In creation area there is already one item (background rectangle) at the
    // start. So it has one item more

    return QTest::qWaitFor(
        [&]() { return numCreationAreaItems() == num_items + 1 && numProjectTableViewItems() == num_items; });
}

bool TestHelperFunctions::compareNumAnimations(const QString item_name, const qint32 num_animations)
{
    const auto item_list = m_project_items_model->findItems(item_name);

    if (item_list.size() != 1) {
        qCritical() << "Item can't be found or name is not unique.";
        return false;
    }

    const auto item = dynamic_cast<ItemModelItem*>(item_list.first());
    const auto item_observer = item->itemObserver();

    return QTest::qWaitFor([&]() { return item_observer->animations().size() == num_animations; });
}

bool TestHelperFunctions::renderVideo(const QString& render_file) const
{
    QSignalSpy finishedVideoRenderingSpy(m_quick_window, SIGNAL(renderingVideoFinished()));

    if (QFile::exists(render_file)) {
        QFile::remove(render_file);
    }
    auto render_action_item = getChild<QObject*>("MVARenderProjectAction");
    QMetaObject::invokeMethod(render_action_item, "trigger");

    auto render_file_dialog = getChild<QObject*>("MVARenderFileDialog");
    if (!QTest::qWaitFor([&]() { return render_file_dialog->property("visible").toBool(); })) {
        return false;
    }

    render_file_dialog->setProperty("selectedFile", QVariant(QUrl::fromLocalFile(render_file)));
    QMetaObject::invokeMethod(render_file_dialog, "simulateAccepted", Qt::DirectConnection);

    return finishedVideoRenderingSpy.wait(60000);
}

QImage TestHelperFunctions::extractImage(const QString& video_file) const
{
    QFile extracted_frame_file("extracted_frame.png");
    QProcess ffmpeg_extract_frame;
    ffmpeg_extract_frame.start("ffmpeg",
        QStringList {} << "-y"
                       << "-i" << video_file << "-frames:v"
                       << "1" << extracted_frame_file.fileName());

    if (!ffmpeg_extract_frame.waitForFinished()) {
        qCritical() << "Couldn't extract frame with ffmpeg";
        return QImage();
    }
    return QImage(extracted_frame_file.fileName());
}

bool TestHelperFunctions::loadFile(const QString& full_file_path) const
{
    if (!QFile::exists(full_file_path)) {
        return false;
    }

    auto load_action_item = getChild<QObject*>("MVALoadProjectAction");
    QMetaObject::invokeMethod(load_action_item, "trigger");

    auto load_file_dialog = getChild<QObject*>("MVALoadFileDialog");
    if (!QTest::qWaitFor([&]() { return load_file_dialog->property("visible").toBool(); })) {
        return false;
    }

    load_file_dialog->setProperty("selectedFile", QVariant(QUrl::fromLocalFile(full_file_path)));
    QMetaObject::invokeMethod(load_file_dialog, "simulateAccepted", Qt::QueuedConnection);

    return QTest::qWaitFor([&]() { return !load_file_dialog->property("visible").toBool(); });
}

QString TestHelperFunctions::copyFileToTestDir(const QFile& file, const QString& new_file_name) const
{
    if (!file.exists()) {
        return QString();
    }

    const QString file_path = TestHelperFunctions::absoluteFilePath(new_file_name);
    if (QFile::exists(file_path)) {
        QFile::remove(file_path);
    }

    if (!QFile::copy(file.fileName(), file_path)) { // NOLINT, false positive
        return QString();
    }
    QFile::setPermissions(file_path, QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    return file_path;
}

void TestHelperFunctions::saveFile() const
{
    auto save_action_item = getChild<QObject*>("MVASaveProjectAction");
    QMetaObject::invokeMethod(save_action_item, "trigger");
}

bool TestHelperFunctions::saveFileAs(const QString& full_file_path) const
{
    auto save_as_action_item = getChild<QObject*>("MVASaveProjectAsAction");
    QMetaObject::invokeMethod(save_as_action_item, "trigger");

    if (QFile::exists(full_file_path)) {
        QFile::remove(full_file_path);
    }

    auto save_file_dialog = getChild<QObject*>("MVASaveFileDialog");
    if (!save_file_dialog->property("visible").toBool()) {
        return false;
    }

    save_file_dialog->setProperty("selectedFile", QVariant(QUrl::fromLocalFile(full_file_path)));
    QMetaObject::invokeMethod(save_file_dialog, "simulateAccepted", Qt::DirectConnection);

    return true;
}

QString TestHelperFunctions::absoluteFilePath(const QString file_name)
{
    const QString save_dir = "test_files";
    QDir current_dir = QDir::current();
    current_dir.mkdir(save_dir);
    current_dir.cd(save_dir);

    return current_dir.absoluteFilePath(file_name);
}

QPoint TestHelperFunctions::itemCenter(QQuickItem* item) const
{
    auto item_center = item->mapToScene(QPoint(0, 0)).toPoint();

    item_center.rx() += item->width() / 2;
    item_center.ry() += item->height() / 2;

    return item_center;
}

void TestHelperFunctions::addAnimationToItem(const qint32 item_number, const qreal start_time, const qreal duration)
{
    clickItem(getQuickItem(item_number), Qt::MouseButton::RightButton);
    auto item_context_menu = getChild<QObject*>("MVAItemContextMenu");
    auto item_context_menu_add_animation = getChild<QObject*>("MVAItemContextMenuAddAnimation");

    QVERIFY(QTest::qWaitFor([&]() { return item_context_menu->property("visible").toBool(); }));
    QVERIFY(QTest::qWaitFor([&]() { return item_context_menu_add_animation->property("visible").toBool(); }));

    QMetaObject::invokeMethod(item_context_menu_add_animation, "simulateClicked");

    auto add_animation_dialog = getChild<QObject*>("MVAAnimationDialog");
    QVERIFY(QTest::qWaitFor([&]() { return add_animation_dialog->property("visible").toBool(); }));

    QMetaObject::invokeMethod(add_animation_dialog, "simulateAccepted", Q_ARG(QVariant, QVariant(start_time)),
        Q_ARG(QVariant, QVariant(duration)));
}
