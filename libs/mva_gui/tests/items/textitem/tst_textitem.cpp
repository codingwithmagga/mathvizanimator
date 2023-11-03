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
  void paintTestScaled();

 private:
  const qreal m_text_x = 46;
  const qreal m_text_y = 98;
  const qreal m_text_width = 124;
  const qreal m_text_height = 276;

  const QString m_text_color = "#0000ff";
  const QString m_text_name = "test_text_name";

  QQuickItem m_text_parent_item;
  TextItem m_text_item;
};

void TestTextItem::initTestCase() {
  m_text_parent_item.setX(m_text_x);
  m_text_parent_item.setY(m_text_y);
  m_text_parent_item.setWidth(m_text_width);
  m_text_parent_item.setHeight(m_text_height);

  m_text_item.setParentItem(&m_text_parent_item);
  m_text_item.setColor(QColor(m_text_color));
  m_text_item.setName(m_text_name);
}

void TestTextItem::toJsonTest() {
  QJsonObject expected_json;
  expected_json["x"] = m_text_x;
  expected_json["y"] = m_text_y;
  expected_json["width"] = m_text_width;
  expected_json["height"] = m_text_height;
  expected_json["item.color"] = m_text_color;
  expected_json["item.name"] = m_text_name;
  expected_json["item.file"] = "qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml";
  expected_json["item.latexSource"] = "";
  expected_json["item.svgFile"] = m_text_item.getSvgFile();

  const auto rect_json = m_text_item.toJson();

  QCOMPARE(rect_json, expected_json);
}

void TestTextItem::latexRenderTest() {
  const QDir appPath =
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

  const QString test_latex = R"($\theta_1 = \delta + \epsilon$)";
  m_text_item.setLatexSource(test_latex);

  QCOMPARE(m_text_item.getLatexSource(), test_latex);
  QCOMPARE(m_text_item.getSvgFile(),
           appPath.absoluteFilePath("0f35255d1355d2c32390101bf57ff4d0.svg"));
}

void TestTextItem::paintTest() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");
  QPainter painter(&image);

  painter.save();
  painter.translate(m_text_item.parentItem()->position());
  m_text_item.paint(&painter);
  painter.restore();

  expected_image.load("://test_images/test_text_image.png");

  QCOMPARE(image, expected_image);
}

void TestTextItem::paintTestScaled() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");
  QPainter painter(&image);

  m_text_item.setScaleText(2.4);

  painter.save();
  painter.translate(m_text_item.parentItem()->position());
  m_text_item.paint(&painter);
  painter.restore();

  expected_image.load("://test_images/test_text_image_scaled.png");

  QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestTextItem)
#include "tst_textitem.moc"
