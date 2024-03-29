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
    void saveProjectFromScratch();
    void saveProjectFromFile();
    void saveProjectWithoutFile();
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
    const QString file_path = m_helper_functions->copyFileToTestDir(test_save_file, "test_load_file.json");
    QVERIFY(m_helper_functions->loadFile(file_path));

    const auto main_window_handler = m_helper_functions->mainWindowHandler();

    QVERIFY(m_helper_functions->compareNumItems(3));
    QVERIFY(m_helper_functions->compareNumAnimations("rect", 2));
    QVERIFY(m_helper_functions->compareNumAnimations("circle", 1));
    QVERIFY(m_helper_functions->compareNumAnimations("text", 0));
    QCOMPARE(main_window_handler->property("project_size").toSize(), QSize(1200, 800));
    QCOMPARE(main_window_handler->property("fps").toInt(), 32);
    QCOMPARE(main_window_handler->property("video_length").toInt(), 8);
    QCOMPARE(main_window_handler->property("background_color").value<QColor>(), QColor("yellow"));

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

void MenuFileIntegrationTest::saveProjectFromScratch()
{
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    const QString test_save_file_absolute_path = TestHelperFunctions::absoluteFilePath("test_save_file.json");
    m_helper_functions->saveFileAs(test_save_file_absolute_path);

    m_helper_functions->dragAndDropCurrentItem(QPoint(200, 200));
    m_helper_functions->saveFile();

    SaveFileHandler save_file_handler;
    const auto load_json_object = save_file_handler.loadJSON(QFileInfo(test_save_file_absolute_path)).object();
    QVERIFY(load_json_object.value("item_0").isObject());
    QVERIFY(load_json_object.value("item_1").isObject());
}

void MenuFileIntegrationTest::saveProjectFromFile()
{
    QFile test_save_file(":/integrations_tests_data/test_save_file.json");
    const QString file_path = m_helper_functions->copyFileToTestDir(test_save_file, "test_load_file.json");
    QVERIFY(m_helper_functions->loadFile(file_path));

    m_helper_functions->dragAndDropCurrentItem(QPoint(200, 200));
    m_helper_functions->saveFile();

    SaveFileHandler save_file_handler;
    const auto load_json_object = save_file_handler.loadJSON(QFileInfo(file_path)).object();
    QVERIFY(load_json_object.value("item_0").isObject());
    QVERIFY(load_json_object.value("item_1").isObject());
    QVERIFY(load_json_object.value("item_2").isObject());
    QVERIFY(load_json_object.value("item_3").isObject());
}

void MenuFileIntegrationTest::saveProjectWithoutFile()
{
    m_helper_functions->dragAndDropCurrentItem(QPoint(200, 200));
    m_helper_functions->saveFile();

    auto save_file_dialog = m_helper_functions->getChild<QObject*>("MVASaveFileDialog");

    QVERIFY(QTest::qWaitFor([&]() { return save_file_dialog->property("visible").toBool(); }));
}

void MenuFileIntegrationTest::saveAsProject()
{
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    QVERIFY(m_helper_functions->compareNumItems(1));
    m_helper_functions->addAnimationToItem(0, 1.0, 2.0);
    m_helper_functions->changePropertyValue(0, "opacity", "0.0");
    m_helper_functions->changeTime(1.7);

    const QString test_save_file_absolute_path = TestHelperFunctions::absoluteFilePath("test_save_file.json");
    m_helper_functions->saveFileAs(test_save_file_absolute_path);

    QVERIFY(QFile::exists(test_save_file_absolute_path));

    SaveFileHandler save_file_handler;
    const auto load_json_object = save_file_handler.loadJSON(QFileInfo(test_save_file_absolute_path)).object();
    QVERIFY(load_json_object.contains("item_0"));
    QVERIFY(load_json_object.contains("mva-version"));
    QVERIFY(load_json_object.contains("project-settings"));
    QVERIFY(load_json_object.value("item_0").isObject());
    QVERIFY(load_json_object.value("project-settings").isObject());

    QCOMPARE(load_json_object.value("mva-version").toString(), "0.0.1");

    const auto item_save_data = load_json_object.value("item_0").toObject();
    QCOMPARE(item_save_data.value("abstract_item.opacity").toString(), "0");

    const auto project_settings_object = load_json_object.value("project-settings").toObject();
    QCOMPARE(project_settings_object.value("width").toInt(), 1024);
    QCOMPARE(project_settings_object.value("height").toInt(), 768);
    QCOMPARE(project_settings_object.value("fps").toInt(), 24);
    QCOMPARE(project_settings_object.value("video_length").toInt(), 5);
    QCOMPARE(project_settings_object.value("background_color").toString(), QColor("black").name());
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
