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
#include <QSignalSpy>
#include <QTest>

#include "circleitem.h"
#include "rectangle.h"
#include "renderer.h"
#include "textitem.h"

class TestRenderer : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void createImage_data();
  void createImage();

  void render_data();
  void render();

  void multipleRendering();

  void setIndividualProjectSettings();

  void cleanupTestCase();

 private:
  QList<AbstractItem*> m_item_list;
};

void TestRenderer::initTestCase() {
  const qreal width_1 = 122;
  const qreal height_1 = width_1;
  const qreal width_2 = 111;
  const qreal height_2 = 82;

  auto parent_item_1 = new QQuickItem();
  parent_item_1->setX(55);
  parent_item_1->setY(65);
  parent_item_1->setHeight(width_1);
  parent_item_1->setWidth(height_1);

  auto parent_item_2 = new QQuickItem();
  parent_item_2->setX(455);
  parent_item_2->setY(265);
  parent_item_2->setHeight(width_2);
  parent_item_2->setWidth(height_2);

  auto parent_item_3 = new QQuickItem();
  parent_item_3->setX(848);
  parent_item_3->setY(165);

  auto circle = new CircleItem();
  circle->setWidth(width_1);
  circle->setHeight(height_1);
  circle->setParentItem(parent_item_1);
  circle->setColor("red");
  circle->setOpacity(0.7);

  auto rect = new RectangleItem();
  rect->setWidth(width_2);
  rect->setHeight(height_2);
  rect->setParentItem(parent_item_2);
  rect->setColor("blue");
  rect->setOpacity(0.7);
  rect->setRotation(43);

  auto tex = new TextItem;
  tex->setLatexSource("Hello $\\delta=\\epsilon$");
  tex->setScaleText(3);
  tex->setParentItem(parent_item_3);
  tex->setOpacity(0.43);
  tex->setRotation(-23);

  m_item_list.push_back(circle);
  m_item_list.push_back(rect);
  m_item_list.push_back(tex);
}

void TestRenderer::createImage_data() {
  QTest::addColumn<qint32>("height");
  QTest::addColumn<qint32>("width");
  QTest::addColumn<QImage>("test_frame_image");

  QImage default_frame("://test_images/test_img_default.png");
  QImage mod_frame("://test_images/test_img_mod.png");

  Renderer::ProjectSettings defaultProjectSettings;

  QTest::newRow("default_setup")
      << defaultProjectSettings.height << defaultProjectSettings.width
      << default_frame;
  QTest::newRow("mod_values") << 750 << 1250 << mod_frame;
}

void TestRenderer::createImage() {
  QFETCH(qint32, height);
  QFETCH(qint32, width);
  QFETCH(QImage, test_frame_image);

  Renderer renderer;

  Renderer::ProjectSettings project_settings;
  project_settings.width = width;
  project_settings.height = height;

  renderer.setProjectSettings(project_settings);
  const auto rendered_image = renderer.createImage(m_item_list);

  QCOMPARE(rendered_image, test_frame_image);
}

void TestRenderer::render_data() {
  QTest::addColumn<qint32>("height");
  QTest::addColumn<qint32>("width");
  QTest::addColumn<qint32>("fps");
  QTest::addColumn<qint32>("video_length");
  QTest::addColumn<QFileInfo>("video_file");
  QTest::addColumn<QImage>("test_frame_image");

  QFileInfo home_dir_video(QDir::home(), "test_video_home.mp4");
  QFileInfo temp_dir_video(QDir::temp(), "test_video_temp.mp4");

  QImage default_frame("://test_images/test_frame_default.png");
  QImage mod_frame("://test_images/test_frame_mod.png");

  Renderer::ProjectSettings defaultProjectSettings;

  QTest::newRow("default_setup")
      << defaultProjectSettings.height << defaultProjectSettings.width
      << defaultProjectSettings.fps << defaultProjectSettings.video_length
      << home_dir_video << default_frame;
  QTest::newRow("mod_values")
      << 750 << 1250 << 32 << 10 << temp_dir_video << mod_frame;
}

void TestRenderer::render() {
  QFETCH(qint32, height);
  QFETCH(qint32, width);
  QFETCH(qint32, fps);
  QFETCH(qint32, video_length);
  QFETCH(QFileInfo, video_file);
  QFETCH(QImage, test_frame_image);

  Renderer renderer;

  QSignalSpy spy(&renderer, &Renderer::finishedRendering);

  connect(
      &renderer, &Renderer::finishedRendering, this,
      [&height, &width, &fps, &video_length,
       &test_frame_image](const QFileInfo& file) {
        QVERIFY(file.exists());

        QMediaPlayer media_player;
        media_player.setSource(QUrl(file.absoluteFilePath()));

        QCOMPARE(media_player.metaData()
                     .value(QMediaMetaData::VideoFrameRate)
                     .toInt(),
                 fps);
        QCOMPARE(media_player.metaData()
                     .value(QMediaMetaData::Duration)
                     .toLongLong(),
                 video_length * 1000);
        QCOMPARE(
            media_player.metaData().value(QMediaMetaData::Resolution).toSize(),
            QSize(width, height));

        QFile extracted_frame_file("extracted_frame_" + QString::number(width) +
                                   "x" + QString::number(height) + ".png");
        QProcess ffmpeg_extract_frame;
        ffmpeg_extract_frame.start(
            "ffmpeg", QStringList{} << "-y"
                                    << "-i" << file.absoluteFilePath()
                                    << "-vframes"
                                    << "1" << extracted_frame_file.fileName());

        QVERIFY(ffmpeg_extract_frame.waitForFinished());
        QCOMPARE(QImage(extracted_frame_file.fileName()), test_frame_image);
      });

  Renderer::ProjectSettings project_settings;
  project_settings.width = width;
  project_settings.height = height;
  project_settings.fps = fps;
  project_settings.video_length = video_length;

  if (QFile::exists(video_file.absoluteFilePath())) {
    QFile(video_file.absoluteFilePath()).remove();
  }

  renderer.setProjectSettings(project_settings);
  renderer.render(m_item_list, video_file);

  QVERIFY(spy.wait(60000));
}

void TestRenderer::multipleRendering() {
  Renderer renderer;

  QSignalSpy spy(&renderer, &Renderer::finishedRendering);
  QFileInfo home_dir_video(QDir::home(), "test_video_home.mp4");

  renderer.render(m_item_list, home_dir_video);
  QVERIFY(spy.wait(60000));

  auto parent_item = new QQuickItem();
  parent_item->setX(312);
  parent_item->setY(444);

  auto circle = new CircleItem(parent_item);
  circle->setWidth(121);
  circle->setHeight(121);
  circle->setColor("green");
  circle->setOpacity(0.89);

  m_item_list.append(circle);

  connect(
      &renderer, &Renderer::finishedRendering, this, [](const QFileInfo& file) {
        QVERIFY(file.exists());

        QFile extracted_frame_file("extracted_frame_multi.png");
        QProcess ffmpeg_extract_frame;
        ffmpeg_extract_frame.start(
            "ffmpeg", QStringList{} << "-y"
                                    << "-i" << file.absoluteFilePath()
                                    << "-vframes"
                                    << "1" << extracted_frame_file.fileName());

        QImage test_frame_image_multi("://test_images/test_frame_multi.png");

        QVERIFY(ffmpeg_extract_frame.waitForFinished());
        QCOMPARE(QImage(extracted_frame_file.fileName()),
                 test_frame_image_multi);
      });

  renderer.render(m_item_list, home_dir_video);
  QVERIFY(spy.wait(60000));
}

void TestRenderer::setIndividualProjectSettings() {
  Renderer renderer;
  const qint32 width = 1200;
  const qint32 height = 888;
  const qint32 fps = 15;
  const qint32 video_length = 12;

  renderer.setWidth(width);
  renderer.setHeight(height);
  renderer.setFPS(fps);
  renderer.setVideoLength(video_length);

  const auto renderer_project_settings = renderer.projectSettings();
  QCOMPARE(renderer_project_settings.width, width);
  QCOMPARE(renderer_project_settings.height, height);
  QCOMPARE(renderer_project_settings.fps, fps);
  QCOMPARE(renderer_project_settings.video_length, video_length);
}

void TestRenderer::cleanupTestCase() { qDeleteAll(m_item_list); }

QTEST_MAIN(TestRenderer)
#include "tst_renderer.moc"
