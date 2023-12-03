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

#include "inttesthelperfcts.h"
#include "main.h"
#include "qobjectdefs.h"

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
  QSharedPointer<IntTestHelperFcts> m_helper_fcts;
};

class CloseEventFilter : public QObject {
  Q_OBJECT
 public:
  CloseEventFilter(QObject* parent) : QObject(parent) {}

 signals:
  void closeEventRecevied();

 protected:
  bool eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Close) {
      emit closeEventRecevied();
    }

    return QObject::eventFilter(obj, event);
  }
};

void MenuFileIntegrationTest::init() {
  m_app_objects = SetupMain::setupApp();

  m_helper_fcts = QSharedPointer<IntTestHelperFcts>(
      new IntTestHelperFcts(m_app_objects.engine));
}

void MenuFileIntegrationTest::clearProject() {
  m_helper_fcts->dragAndDropCurrentItem(QPoint(100, 100));
  IntTestHelperFcts::processEvents();

  // qml_creation_area already has one child item at the beginning, see
  // MainWindow.qml
  QCOMPARE(m_helper_fcts->numCreationAreaItems(), 2);
  QCOMPARE(m_helper_fcts->numProjectTableViewItems(), 1);

  QObject* new_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>("MVANewProjectAction");
  QVERIFY2(new_action_item != nullptr, "New Project action not found");
  QMetaObject::invokeMethod(new_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QCOMPARE(m_helper_fcts->numCreationAreaItems(), 1);
  QCOMPARE(m_helper_fcts->numProjectTableViewItems(), 0);
}

void MenuFileIntegrationTest::loadProject() {
  QFile test_save_file(":/integrations_tests_data/test_save_file.json");
  QVERIFY(test_save_file.exists());

  const QString test_file_name = "test_save_file.json";
  const QString save_dir = "test_save_files";
  QDir current_dir = QDir::current();
  current_dir.mkdir(save_dir);
  current_dir.cd(save_dir);
  const QString test_save_file_absolut_path =
      current_dir.absoluteFilePath(test_file_name);

  test_save_file.copy(test_save_file_absolut_path);
  QVERIFY(QFile::exists(test_save_file_absolut_path));

  QObject* load_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>("MVALoadProjectAction");
  QVERIFY2(load_action_item != nullptr, "Load Project action not found");
  QMetaObject::invokeMethod(load_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QObject* load_file_dialog =
      m_helper_fcts->rootWindow()->findChild<QObject*>("MVALoadFileDialog");
  QVERIFY2(load_file_dialog != nullptr, "Load File Dialog not found");
  QVERIFY2(load_file_dialog->property("visible").toBool(),
           "Load File Dialog not visible");

  load_file_dialog->setProperty(
      "selectedFile", QVariant(QUrl("file://" + test_save_file_absolut_path)));
  QMetaObject::invokeMethod(load_file_dialog, "simulateAccepted",
                            Qt::DirectConnection);
  IntTestHelperFcts::processEvents();

  QCOMPARE(m_helper_fcts->numCreationAreaItems(), 4);
  QCOMPARE(m_helper_fcts->numProjectTableViewItems(), 3);
}

void MenuFileIntegrationTest::saveAsProject() {
  m_helper_fcts->dragAndDropCurrentItem(QPoint(100, 100));
  IntTestHelperFcts::processEvents();

  QObject* save_as_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVASaveProjectAsAction");
  QVERIFY2(save_as_action_item != nullptr, "Save As Project action not found");

  QMetaObject::invokeMethod(save_as_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  const QString test_file_name = "test_save_as_file.json";
  const QString save_dir = "test_save_files";
  QDir current_dir = QDir::current();
  current_dir.mkdir(save_dir);
  current_dir.cd(save_dir);
  const QString test_save_file_absolut_path =
      current_dir.absoluteFilePath(test_file_name);

  if (QFile::exists(test_save_file_absolut_path)) {
    QFile::remove(test_save_file_absolut_path);
  }

  QObject* save_file_dialog =
      m_helper_fcts->rootWindow()->findChild<QObject*>("MVASaveFileDialog");
  QVERIFY2(save_file_dialog != nullptr, "Save File Dialog not found");
  QVERIFY2(save_file_dialog->property("visible").toBool(),
           "Save File Dialog not visible");

  save_file_dialog->setProperty(
      "selectedFile", QVariant(QUrl("file://" + test_save_file_absolut_path)));
  QMetaObject::invokeMethod(save_file_dialog, "simulateAccepted",
                            Qt::DirectConnection);
  IntTestHelperFcts::processEvents();

  QVERIFY2(QFile::exists(test_save_file_absolut_path),
           "Save file wasn't created!");
}

void MenuFileIntegrationTest::quitApp() {
  QObject* quit_app_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>("MVAQuitAppAction");
  QVERIFY2(quit_app_action_item != nullptr, "Quit App action not found");
  CloseEventFilter* closeFilter =
      new CloseEventFilter(m_helper_fcts->rootWindow());
  m_helper_fcts->rootWindow()->installEventFilter(closeFilter);
  QSignalSpy closeEventySpy(closeFilter, &CloseEventFilter::closeEventRecevied);

  QMetaObject::invokeMethod(quit_app_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QCOMPARE(closeEventySpy.count(), 1);
}

void MenuFileIntegrationTest::cleanup() {
  m_helper_fcts.clear();

  m_app_objects.engine.clear();
  m_app_objects.mainlogic.clear();
}

QTEST_MAIN(MenuFileIntegrationTest)
#include "tst_menu_file.moc"
