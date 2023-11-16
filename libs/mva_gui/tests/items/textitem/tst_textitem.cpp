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

#include <QPainter>
#include <QTest>

#include "textitem.h"

class TestTextItem : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void toJsonTest();
  void latexRenderTest();
  void paintTest();

 private:
  const qreal m_text_x = 46;
  const qreal m_text_y = 98;

  const QString m_text_color = "#0000ff";
  const QString m_text_name = "test_text_name";
  const qreal m_text_rotation = 43;
  const qreal m_text_opacity = 0.8;
  const qreal m_text_scale = 4.2;

  QQuickItem m_text_parent_item;
  TextItem m_text_item;
};

void TestTextItem::initTestCase() {
  m_text_parent_item.setX(m_text_x);
  m_text_parent_item.setY(m_text_y);

  m_text_item.setParentItem(&m_text_parent_item);
  m_text_item.setColor(QColor(m_text_color));
  m_text_item.setName(m_text_name);
  m_text_item.setRotation(m_text_rotation);
  m_text_item.setOpacity(m_text_opacity);
  m_text_item.setScaleText(m_text_scale);
}

void TestTextItem::toJsonTest() {
  QJsonObject expected_json;
  expected_json["x"] = QString::number(m_text_x);
  expected_json["y"] = QString::number(m_text_y);
  expected_json["item.name"] = m_text_name;
  expected_json["item.rotation"] = QString::number(m_text_rotation);
  expected_json["item.opacity"] = QString::number(m_text_opacity);
  expected_json["item.scaleText"] = QString::number(m_text_scale);
  expected_json["item.file"] = "qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml";
  expected_json["item.latexSource"] = "";

  const auto text_json = m_text_item.toJson();

  QCOMPARE(text_json, expected_json);
}

void TestTextItem::latexRenderTest() {
  const QDir appPath =
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

  auto svg_file =
      QFile(appPath.absoluteFilePath("0f35255d1355d2c32390101bf57ff4d0.svg"));
  if (svg_file.exists()) {
    QVERIFY(svg_file.remove());
  }

  const QString test_latex = R"($\theta_1 = \delta + \epsilon$)";
  m_text_item.setLatexSource(test_latex);

  QVERIFY(svg_file.exists());
  QCOMPARE(m_text_item.getLatexSource(), test_latex);
  QCOMPARE(m_text_item.getSvgFile(),
           appPath.absoluteFilePath(svg_file.fileName()));
}

void TestTextItem::paintTest() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");

  QPainter painter(&image);
  m_text_item.paintItem(&painter);

  expected_image.load("://test_images/test_text_image.png");
  QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestTextItem)
#include "tst_textitem.moc"
