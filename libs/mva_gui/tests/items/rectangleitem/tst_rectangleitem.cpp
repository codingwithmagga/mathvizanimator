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
  const qreal m_test_x = 46;
  const qreal m_test_y = 98;
  const qreal m_test_width = 276;
  const qreal m_test_height = 124;

  const QString m_test_color = "#0000ff";
  const QString m_test_name = "test_rect_name";

  QQuickItem m_test_parent_item;
  RectangleItem m_test_rect;
};

void TestRectangleItem::initTestCase() {
  m_test_parent_item.setX(m_test_x);
  m_test_parent_item.setY(m_test_y);
  m_test_parent_item.setWidth(m_test_width);
  m_test_parent_item.setHeight(m_test_height);

  m_test_rect.setParentItem(&m_test_parent_item);
  m_test_rect.setColor(QColor(m_test_color));
  m_test_rect.setName(m_test_name);

  m_test_rect.setWidth(m_test_width);
  m_test_rect.setHeight(m_test_height);
}

void TestRectangleItem::toJsonTest() {
  QJsonObject expected_json;
  expected_json["x"] = m_test_x;
  expected_json["y"] = m_test_y;
  expected_json["width"] = m_test_width;
  expected_json["height"] = m_test_height;
  expected_json["item.color"] = m_test_color;
  expected_json["item.name"] = m_test_name;
  expected_json["item.file"] =
      "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml";

  const auto rect_json = m_test_rect.toJson();

  QCOMPARE(rect_json, expected_json);
}

void TestRectangleItem::paintTest() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");
  QPainter painter(&image);

  painter.save();
  painter.translate(m_test_rect.parentItem()->position());
  m_test_rect.paint(&painter);
  painter.restore();

  expected_image.load("://test_images/test_rectangle_image.png");

  QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestRectangleItem)
#include "tst_rectangleitem.moc"
