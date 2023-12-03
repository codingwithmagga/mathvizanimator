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

#include "inttesthelperfcts.h"
#include "main.h"
#include "qobjectdefs.h"

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
  SetupMain::SetupObjects m_app_objects;
  QSharedPointer<IntTestHelperFcts> m_helper_fcts;
};

void MenuProjectIntegrationTest::init() {
  m_app_objects = SetupMain::setupApp();

  m_helper_fcts = QSharedPointer<IntTestHelperFcts>(
      new IntTestHelperFcts(m_app_objects.engine));
}

void MenuProjectIntegrationTest::renderProject() {
  m_helper_fcts->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_fcts->dragAndDropCurrentItem(QPoint(300, 180));
  IntTestHelperFcts::processEvents();

  QDir current_dir = QDir::current();
  const QString render_file = current_dir.absoluteFilePath("render_test.mp4");
  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }

  QSignalSpy finishedVideoRenderingSpy(m_helper_fcts->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  QObject* render_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVARenderProjectAction");
  QVERIFY2(render_action_item != nullptr, "Render Project action not found");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  QVERIFY2(finishedVideoRenderingSpy.wait(60000),
           "Finished rendering signal wasn't emitted!");
  QVERIFY2(QFile::exists(render_file), "Render file wasn't created!");
}

void MenuProjectIntegrationTest::createSnapshot() {
  m_helper_fcts->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_fcts->dragAndDropCurrentItem(QPoint(300, 180));
  IntTestHelperFcts::processEvents();

  QDir current_dir = QDir::current();
  const QString snapshot_file = current_dir.absoluteFilePath("snapshot.png");
  if (QFile::exists(snapshot_file)) {
    QFile::remove(snapshot_file);
  }

  QObject* snapshot_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVACreateSnapshotAction");
  QVERIFY2(snapshot_action_item != nullptr,
           "Snapshot Project action not found");
  QMetaObject::invokeMethod(snapshot_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QVERIFY2(QFile::exists(snapshot_file), "Snapshot image wasn't created!");
}

void MenuProjectIntegrationTest::openProjectSettings() {
  m_helper_fcts->dragAndDropCurrentItem(QPoint(100, 100));
  m_helper_fcts->dragAndDropCurrentItem(QPoint(300, 180));
  IntTestHelperFcts::processEvents();

  QObject* open_project_settings_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVAOpenProjectSettingsAction");
  QVERIFY2(open_project_settings_action_item != nullptr,
           "Open Project Settings action not found");
  QMetaObject::invokeMethod(open_project_settings_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QObject* project_settings_popup_object =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVAProjectSettingsPopup");
  QVERIFY2(project_settings_popup_object->property("visible").toBool(),
           "Project Settings Popup not visible");
  IntTestHelperFcts::processEvents();

  const qint32 width = 800;
  const qint32 height = 600;
  const qint32 fps = 32;
  const qint32 video_length = 2;

  QObject* width_input_field =
      project_settings_popup_object->findChild<QObject*>("MVAWidthInputField");
  QVERIFY2(width_input_field != nullptr, "Width input field not found");
  width_input_field->setProperty("text", QVariant(width));
  QObject* height_input_field =
      project_settings_popup_object->findChild<QObject*>("MVAHeightInputField");
  QVERIFY2(height_input_field != nullptr, "Height input field not found");
  height_input_field->setProperty("text", QVariant(height));
  QObject* fps_input_field =
      project_settings_popup_object->findChild<QObject*>("MVAFPSInputField");
  QVERIFY2(fps_input_field != nullptr, "FPS input field not found");
  fps_input_field->setProperty("text", QVariant(fps));
  QObject* video_length_input_field =
      project_settings_popup_object->findChild<QObject*>(
          "MVAVideoLengthInputField");
  QVERIFY2(video_length_input_field != nullptr,
           "Video length input field not found");
  video_length_input_field->setProperty("text", QVariant(video_length));
  IntTestHelperFcts::processEvents();

  auto save_button = project_settings_popup_object->findChild<QQuickItem*>(
      "MVASaveProjectSettingsButton");
  QVERIFY2(save_button != nullptr, "Save button not found");
  m_helper_fcts->clickItem(save_button);
  IntTestHelperFcts::processEvents();

  QSignalSpy finishedVideoRenderingSpy(m_helper_fcts->rootWindow(),
                                       SIGNAL(renderingVideoFinished()));

  QDir current_dir = QDir::current();
  const QString render_file = current_dir.absoluteFilePath("render_test.mp4");
  if (QFile::exists(render_file)) {
    QFile::remove(render_file);
  }

  QObject* render_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVARenderProjectAction");
  QVERIFY2(render_action_item != nullptr, "Render Project action not found");
  QMetaObject::invokeMethod(render_action_item, "trigger");

  QVERIFY2(finishedVideoRenderingSpy.wait(60000),
           "Finished rendering signal wasn't emitted!");
  QVERIFY2(QFile::exists(render_file), "Render file wasn't created!");

  QMediaPlayer media_player;
  media_player.setSource(QUrl(render_file));

  QCOMPARE(
      media_player.metaData().value(QMediaMetaData::VideoFrameRate).toInt(),
      fps);
  QCOMPARE(media_player.metaData().value(QMediaMetaData::Duration).toLongLong(),
           video_length * 1000);
  QCOMPARE(media_player.metaData().value(QMediaMetaData::Resolution).toSize(),
           QSize(width, height));
}

void MenuProjectIntegrationTest::openSVGFolder() {
  QObject* open_svg_folder_action_item =
      m_helper_fcts->rootWindow()->findChild<QObject*>(
          "MVAOpenSVGFolderAction");
  QVERIFY2(open_svg_folder_action_item != nullptr,
           "Open SVG Folder action not found");
  QMetaObject::invokeMethod(open_svg_folder_action_item, "trigger");
  IntTestHelperFcts::processEvents();

  QSKIP(
      "Currently no option to test the open SVG Folder action. Check the log.");
}

void MenuProjectIntegrationTest::cleanup() {
  m_helper_fcts.clear();

  m_app_objects.engine.clear();
  m_app_objects.mainlogic.clear();
}

QTEST_MAIN(MenuProjectIntegrationTest)
#include "tst_menu_project.moc"
