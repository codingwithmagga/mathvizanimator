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

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSignalSpy>
#include <QTest>

#include "main.h"
#include "qobjectdefs.h"
#include "test_helper_functions.h"

class MenuFileIntegrationTest : public QObject {
    Q_OBJECT

  private slots:
    void init();

    void clearProject();
    void loadProject();
    void saveAsProject();
    void quitApp();

    void cleanup();

  private:
    SetupMain::SetupObjects m_app_objects;
    QSharedPointer<TestHelperFunctions> m_helper_functions;
};

class CloseEventFilter : public QObject {
    Q_OBJECT
  public:
    explicit CloseEventFilter(QObject* parent)
        : QObject(parent)
    {
    }

  signals:
    void closeEventReceived();

  protected:
    bool eventFilter(QObject* obj, QEvent* event)
    {
        if (event->type() == QEvent::Close) {
            emit closeEventReceived();
        }

        return QObject::eventFilter(obj, event);
    }
};

void MenuFileIntegrationTest::init()
{
    m_app_objects = SetupMain::setupApp();

    m_helper_functions = QSharedPointer<TestHelperFunctions>(new TestHelperFunctions(m_app_objects.engine));
}

void MenuFileIntegrationTest::clearProject()
{
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    QVERIFY(m_helper_functions->compareNumItems(1));

    auto new_action_item = m_helper_functions->getChild<QObject*>("MVANewProjectAction");
    QMetaObject::invokeMethod(new_action_item, "trigger");

    QVERIFY(m_helper_functions->compareNumItems(0));
}

void MenuFileIntegrationTest::loadProject()
{
    QFile test_save_file(":/integrations_tests_data/test_save_file.json");
    QVERIFY(test_save_file.exists());

    const QString test_load_file_absolute_path = TestHelperFunctions::absoluteFilePath("test_load_file.json");
    if (QFile::exists(test_load_file_absolute_path)) {
        QFile::remove(test_load_file_absolute_path);
    }

    test_save_file.copy(test_load_file_absolute_path);
    QVERIFY(QFile::exists(test_load_file_absolute_path));

    auto load_action_item = m_helper_functions->getChild<QObject*>("MVALoadProjectAction");
    QMetaObject::invokeMethod(load_action_item, "trigger");

    auto load_file_dialog = m_helper_functions->getChild<QObject*>("MVALoadFileDialog");
    QVERIFY(QTest::qWaitFor([&]() { return load_file_dialog->property("visible").toBool(); }));

    load_file_dialog->setProperty("selectedFile", QVariant(QUrl::fromLocalFile(test_load_file_absolute_path)));
    QMetaObject::invokeMethod(load_file_dialog, "simulateAccepted", Qt::QueuedConnection);

    const auto main_window_handler = m_helper_functions->mainWindowHandler();

    QVERIFY(QTest::qWaitFor([&]() { return !load_file_dialog->property("visible").toBool(); }));
    QVERIFY(m_helper_functions->compareNumItems(3));
    QVERIFY(m_helper_functions->compareNumAnimations("rect", 2));
    QVERIFY(m_helper_functions->compareNumAnimations("circle", 1));
    QVERIFY(m_helper_functions->compareNumAnimations("text", 0));
    QCOMPARE(main_window_handler->property("pixel_width").toInt(), 1200);
    QCOMPARE(main_window_handler->property("pixel_height").toInt(), 800);
    QCOMPARE(main_window_handler->property("fps").toInt(), 32);
    QCOMPARE(main_window_handler->property("video_length").toInt(), 8);

    // Check if item is clickable, see Issue #37
    auto rect_item = m_helper_functions->getQuickItem(0);
    auto circle_item = m_helper_functions->getQuickItem(1);

    m_helper_functions->clickItem(rect_item);
    QVERIFY(QTest::qWaitFor([&]() { return m_helper_functions->numPropertyTableViewItems() > 0; }));
    QCOMPARE(m_helper_functions->getPropertyValue("name"), "rect");

    m_helper_functions->clickItem(circle_item);
    QVERIFY(QTest::qWaitFor([&]() { return m_helper_functions->numPropertyTableViewItems() > 0; }));
    QCOMPARE(m_helper_functions->getPropertyValue("name"), "circle");
}

void MenuFileIntegrationTest::saveAsProject()
{
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    QVERIFY(m_helper_functions->compareNumItems(1));

    auto save_as_action_item = m_helper_functions->getChild<QObject*>("MVASaveProjectAsAction");
    QMetaObject::invokeMethod(save_as_action_item, "trigger");

    const QString test_save_file_absolute_path = TestHelperFunctions::absoluteFilePath("test_save_file.json");
    if (QFile::exists(test_save_file_absolute_path)) {
        QFile::remove(test_save_file_absolute_path);
    }

    auto save_file_dialog = m_helper_functions->getChild<QObject*>("MVASaveFileDialog");
    QVERIFY(save_file_dialog->property("visible").toBool());

    save_file_dialog->setProperty("selectedFile", QVariant(QUrl::fromLocalFile(test_save_file_absolute_path)));
    QMetaObject::invokeMethod(save_file_dialog, "simulateAccepted", Qt::DirectConnection);

    QVERIFY(QFile::exists(test_save_file_absolute_path));

    SaveFileHandler save_file_handler;
    const auto load_json_object = save_file_handler.loadJSON(QFileInfo(test_save_file_absolute_path)).object();
    QVERIFY(load_json_object.contains("item_0"));
    QVERIFY(load_json_object.contains("mva-version"));
    QVERIFY(load_json_object.contains("project-settings"));
    QVERIFY(load_json_object.value("project-settings").isObject());

    const auto project_settings_object = load_json_object.value("project-settings").toObject();
    QCOMPARE(load_json_object.value("mva-version").toString(), "0.0.1");
    QCOMPARE(project_settings_object.value("width").toInt(), 1024);
    QCOMPARE(project_settings_object.value("height").toInt(), 768);
    QCOMPARE(project_settings_object.value("fps").toInt(), 24);
    QCOMPARE(project_settings_object.value("video_length").toInt(), 5);
}

void MenuFileIntegrationTest::quitApp()
{
    auto quit_app_action_item = m_helper_functions->getChild<QObject*>("MVAQuitAppAction");
    CloseEventFilter* closeFilter = new CloseEventFilter(m_helper_functions->rootWindow());
    m_helper_functions->rootWindow()->installEventFilter(closeFilter);
    QSignalSpy closeEventSpy(closeFilter, &CloseEventFilter::closeEventReceived);

    QMetaObject::invokeMethod(quit_app_action_item, "trigger");

    QVERIFY(QTest::qWaitFor([&]() { return closeEventSpy.count() == 1; }));
}

void MenuFileIntegrationTest::cleanup()
{
    m_helper_functions.clear();

    m_app_objects.engine.clear();
    m_app_objects.mainlogic.clear();
}

QTEST_MAIN(MenuFileIntegrationTest)
#include "tst_menu_file.moc"
