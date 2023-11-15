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

#include "rectangle.h"

class TestRectangleItem : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void toJsonTest();
  void paintTest();

 private:
  const qreal m_rect_x = 46;
  const qreal m_rect_y = 98;
  const qreal m_rect_width = 276;
  const qreal m_rect_height = 124;

  const QString m_rect_color = "#0000ff";
  const QString m_rect_name = "test_rect_name";

  QQuickItem m_rect_parent_item;
  RectangleItem m_rect_item;
};

void TestRectangleItem::initTestCase() {
  m_rect_parent_item.setX(m_rect_x);
  m_rect_parent_item.setY(m_rect_y);
  m_rect_parent_item.setWidth(m_rect_width);
  m_rect_parent_item.setHeight(m_rect_height);

  m_rect_item.setParentItem(&m_rect_parent_item);
  m_rect_item.setColor(QColor(m_rect_color));
  m_rect_item.setName(m_rect_name);

  m_rect_item.setWidth(m_rect_width);
  m_rect_item.setHeight(m_rect_height);
}

void TestRectangleItem::toJsonTest() {
  QJsonObject expected_json;
  expected_json["x"] = m_rect_x;
  expected_json["y"] = m_rect_y;
  expected_json["width"] = m_rect_width;
  expected_json["height"] = m_rect_height;
  expected_json["item.color"] = m_rect_color;
  expected_json["item.name"] = m_rect_name;
  expected_json["item.file"] =
      "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml";

  const auto rect_json = m_rect_item.toJson();

  QCOMPARE(rect_json, expected_json);
}

void TestRectangleItem::paintTest() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");
  QPainter painter(&image);

  painter.save();
  painter.translate(m_rect_item.parentItem()->position());
  m_rect_item.paint(&painter);
  painter.restore();

  expected_image.load("://test_images/test_rectangle_image.png");

  QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestRectangleItem)
#include "tst_rectangleitem.moc"
