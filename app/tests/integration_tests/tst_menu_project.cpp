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
  void setProjectSettings();
  void setTextProperty(const QString& name, const qint32 value);

  SetupMain::SetupObjects m_app_objects;
  QSharedPointer<TestHelperFunctions> m_helper_functions;

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
      QDir::current().absoluteFilePath("render_test.mp4");
  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }

  QSignalSpy finishedVideoRenderingSpy(m_helper_functions->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  auto render_action_item =
      m_helper_functions->getChild<QObject*>("MVARenderProjectAction");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  QVERIFY(finishedVideoRenderingSpy.wait(60000));
  QVERIFY(QFile::exists(render_file));
}

void MenuProjectIntegrationTest::createSnapshot() {
  m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_functions->dragAndDropCurrentItem(QPoint(300, 180));
  QVERIFY(m_helper_functions->compareNumItems(2));

  const QString snapshot_file =
      QDir::current().absoluteFilePath("snapshot.png");
  if (QFile::exists(snapshot_file)) {
    QFile::remove(snapshot_file);
  }

  auto snapshot_action_item =
      m_helper_functions->getChild<QObject*>("MVACreateSnapshotAction");
  QMetaObject::invokeMethod(snapshot_action_item, "trigger");

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

  auto project_settings_popup_object =
      m_helper_functions->getChild<QObject*>("MVAProjectSettingsPopup");
  QVERIFY(project_settings_popup_object->property("visible").toBool());

  setProjectSettings();

  QSignalSpy finishedVideoRenderingSpy(m_helper_functions->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  const QString render_file =
      QDir::current().absoluteFilePath("render_test.mp4");
  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }

  auto render_action_item =
      m_helper_functions->getChild<QObject*>("MVARenderProjectAction");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  QVERIFY(finishedVideoRenderingSpy.wait(60000));
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

void MenuProjectIntegrationTest::setProjectSettings() {
  setTextProperty("MVAWidthInputField", m_width);
  setTextProperty("MVAHeightInputField", m_height);
  setTextProperty("MVAFpsInputField", m_fps);
  setTextProperty("MVAVideoLengthInputField", m_video_length);

  auto save_button =
      m_helper_functions->getChild<QQuickItem*>("MVASaveProjectSettingsButton");
  m_helper_functions->clickItem(save_button);
}

void MenuProjectIntegrationTest::setTextProperty(const QString& name,
                                                 const qint32 value) {
  auto object = m_helper_functions->getChild<QObject*>(name);
  object->setProperty("text", QVariant(value));
}

QTEST_MAIN(MenuProjectIntegrationTest)
#include "tst_menu_project.moc"