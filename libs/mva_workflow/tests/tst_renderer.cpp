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

class TestRenderer : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void render();
  void render_data();

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

  auto circle = new CircleItem();
  circle->setWidth(width_1);
  circle->setHeight(height_1);
  circle->setParentItem(parent_item_1);
  circle->setColor("red");

  auto rect = new RectangleItem();
  rect->setWidth(width_2);
  rect->setHeight(height_2);
  rect->setParentItem(parent_item_2);
  rect->setColor("blue");

  m_item_list.push_back(circle);
  m_item_list.push_back(rect);
}

void TestRenderer::render_data() {
  QTest::addColumn<qint32>("height");
  QTest::addColumn<qint32>("width");
  QTest::addColumn<qint32>("fps");
  QTest::addColumn<qint32>("video_length");

  Renderer::ProjectSettings defaultProjectSettings;

  QTest::newRow("default_setup")
      << defaultProjectSettings.height << defaultProjectSettings.width
      << defaultProjectSettings.fps << defaultProjectSettings.video_length;
  QTest::newRow("mod_values") << 600 << 400 << 32 << 10;
}

void TestRenderer::render() {
  QFETCH(qint32, height);
  QFETCH(qint32, width);
  QFETCH(qint32, fps);
  QFETCH(qint32, video_length);

  Renderer renderer;

  QSignalSpy spy(&renderer, &Renderer::finishedRendering);

  connect(
      &renderer, &Renderer::finishedRendering, this,
      [&height, &width, &fps, &video_length](const QFileInfo& file) {
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
      });

  Renderer::ProjectSettings project_settings;
  project_settings.width = width;
  project_settings.height = height;
  project_settings.fps = fps;
  project_settings.video_length = video_length;

  renderer.setProjectSettings(project_settings);
  renderer.render(m_item_list);

  QVERIFY(spy.wait(10000));
}

void TestRenderer::cleanupTestCase() { qDeleteAll(m_item_list); }

QTEST_MAIN(TestRenderer)
#include "tst_renderer.moc"
