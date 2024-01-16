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

#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QSignalSpy>
#include <QTest>

#include "main.h"
#include "qobjectdefs.h"
#include "test_helper_functions.h"

class MenuProjectIntegrationTest : public QObject {
  Q_OBJECT

 private slots:
  void init();

  void renderProject();
  void createSnapshot();
  void openProjectSettings();
  void openSVGFolder();

  void cleanup();

 private:
  bool setProjectSettings();
  void setTextProperty(const QString& name, const qint32 value);
  bool renderProjectToFile(const QString& render_file);

  SetupMain::SetupObjects m_app_objects;
  QSharedPointer<TestHelperFunctions> m_helper_functions;

  QSharedPointer<QSignalSpy> m_finishedVideoRenderingSpy;

  const qint32 m_width = 800;
  const qint32 m_height = 600;
  const qint32 m_fps = 32;
  const qint32 m_video_length = 2;
};

void MenuProjectIntegrationTest::init() {
  m_app_objects = SetupMain::setupApp();

  m_helper_functions = QSharedPointer<TestHelperFunctions>(
      new TestHelperFunctions(m_app_objects.engine));
}

void MenuProjectIntegrationTest::renderProject() {
  m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_functions->dragAndDropCurrentItem(QPoint(300, 180));
  QVERIFY(m_helper_functions->compareNumItems(2));

  const QString render_file =
      QDir::current().absoluteFilePath("render_project_test_video.mp4");

  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }

  QSignalSpy finishedVideoRenderingSpy(m_helper_functions->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  auto render_action_item =
      m_helper_functions->getChild<QObject*>("MVARenderProjectAction");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  auto render_file_dialog =
      m_helper_functions->getChild<QObject*>("MVARenderFileDialog");
  QVERIFY(QTest::qWaitFor(
      [&]() { return render_file_dialog->property("visible").toBool(); }));

  render_file_dialog->setProperty("selectedFile",
                                  QVariant(QUrl::fromLocalFile(render_file)));
  QMetaObject::invokeMethod(render_file_dialog, "simulateAccepted",
                            Qt::QueuedConnection);

  QVERIFY(finishedVideoRenderingSpy.wait(60000));

  QVERIFY(QFile::exists(render_file));
}

void MenuProjectIntegrationTest::createSnapshot() {
  m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_functions->dragAndDropCurrentItem(QPoint(300, 180));
  QVERIFY(m_helper_functions->compareNumItems(2));

  const QString snapshot_file =
      TestHelperFunctions::absoluteFilePath("snapshot.png");
  if (QFile::exists(snapshot_file)) {
    QFile::remove(snapshot_file);
  }

  auto snapshot_action_item =
      m_helper_functions->getChild<QObject*>("MVACreateSnapshotAction");
  QMetaObject::invokeMethod(snapshot_action_item, "trigger");

  auto snapshot_file_dialog =
      m_helper_functions->getChild<QObject*>("MVASnapshotFileDialog");
  QVERIFY(snapshot_file_dialog->property("visible").toBool());

  snapshot_file_dialog->setProperty(
      "selectedFile", QVariant(QUrl::fromLocalFile(snapshot_file)));
  QMetaObject::invokeMethod(snapshot_file_dialog, "simulateAccepted",
                            Qt::DirectConnection);

  QVERIFY(
      QTest::qWaitFor([&]() { return QFile::exists(snapshot_file) == true; }));
}

void MenuProjectIntegrationTest::openProjectSettings() {
  m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_functions->dragAndDropCurrentItem(QPoint(300, 180));
  QVERIFY(m_helper_functions->compareNumItems(2));

  auto open_project_settings_action_item =
      m_helper_functions->getChild<QObject*>("MVAOpenProjectSettingsAction");
  QMetaObject::invokeMethod(open_project_settings_action_item, "trigger");

  QVERIFY(setProjectSettings());

  const QString render_file = TestHelperFunctions::absoluteFilePath(
      "render_project_settings_test_video.mp4");

  QVERIFY(renderProjectToFile(render_file));
  QVERIFY(QFile::exists(render_file));

  QMediaPlayer media_player;
  media_player.setSource(QUrl(render_file));

  QCOMPARE(media_player.metaData().value(QMediaMetaData::Duration).toLongLong(),
           m_video_length * 1000);
  QCOMPARE(media_player.metaData().value(QMediaMetaData::Resolution).toSize(),
           QSize(m_width, m_height));
  QCOMPARE(
      media_player.metaData().value(QMediaMetaData::VideoFrameRate).toInt(),
      m_fps);
}

void MenuProjectIntegrationTest::openSVGFolder() {
  auto open_svg_folder_action_item =
      m_helper_functions->getChild<QObject*>("MVAOpenSVGFolderAction");
  QMetaObject::invokeMethod(open_svg_folder_action_item, "trigger");

  QSKIP(
      "Currently no option to test the open SVG Folder action. Check the "
      "log. On Windows and MacOS log output is false, because the dir doesn't "
      "exists. It relates to the name of the test. Fix is not "
      "important currently.");
}

void MenuProjectIntegrationTest::cleanup() {
  m_helper_functions.clear();

  m_app_objects.engine.clear();
  m_app_objects.mainlogic.clear();
}

bool MenuProjectIntegrationTest::setProjectSettings() {
  auto project_settings_popup_object =
      m_helper_functions->getChild<QObject*>("MVAProjectSettingsDialog");
  if (!QTest::qWaitFor([&]() {
        return project_settings_popup_object->property("visible").toBool();
      })) {
    return false;
  }

  setTextProperty("MVAWidthInputField", m_width);
  setTextProperty("MVAHeightInputField", m_height);
  setTextProperty("MVAFpsInputField", m_fps);
  setTextProperty("MVAVideoLengthInputField", m_video_length);

  if (!QMetaObject::invokeMethod(project_settings_popup_object,
                                 "simulateAccepted", Qt::DirectConnection)) {
    return false;
  }

  return QTest::qWaitFor([&]() {
    return !project_settings_popup_object->property("visible").toBool();
  });
}

void MenuProjectIntegrationTest::setTextProperty(const QString& name,
                                                 const qint32 value) {
  auto object = m_helper_functions->getChild<QObject*>(name);
  object->setProperty("text", QVariant(value));
}

bool MenuProjectIntegrationTest::renderProjectToFile(
    const QString& render_file) {
  QSignalSpy finishedVideoRenderingSpy(m_helper_functions->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }
  auto render_action_item =
      m_helper_functions->getChild<QObject*>("MVARenderProjectAction");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  auto render_file_dialog =
      m_helper_functions->getChild<QObject*>("MVARenderFileDialog");
  if (!QTest::qWaitFor(
          [&]() { return render_file_dialog->property("visible").toBool(); })) {
    return false;
  }

  render_file_dialog->setProperty("selectedFile",
                                  QVariant(QUrl::fromLocalFile(render_file)));
  QMetaObject::invokeMethod(render_file_dialog, "simulateAccepted",
                            Qt::DirectConnection);

  return finishedVideoRenderingSpy.wait(60000);
}

QTEST_MAIN(MenuProjectIntegrationTest)
#include "tst_menu_project.moc"
