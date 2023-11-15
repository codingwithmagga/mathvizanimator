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

#include "circleitem.h"

class TestCircleItem : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void toJsonTest();
  void paintTest();

 private:
  const qreal m_circle_x = 123;
  const qreal m_circle_y = 212;
  const qreal m_circle_width = 150;
  const qreal m_circle_height = 150;

  const QString m_circle_color = "#0000ff";
  const QString m_circle_name = "test_circle_name";
  const qreal m_circle_opacity = 0.6;
  const qreal m_circle_rotation = 32;

  QQuickItem m_circle_parent_item;
  CircleItem m_circle_item;
};

void TestCircleItem::initTestCase() {
  m_circle_parent_item.setX(m_circle_x);
  m_circle_parent_item.setY(m_circle_y);
  m_circle_parent_item.setWidth(m_circle_width);
  m_circle_parent_item.setHeight(m_circle_height);

  m_circle_item.setParentItem(&m_circle_parent_item);
  m_circle_item.setColor(QColor(m_circle_color));
  m_circle_item.setName(m_circle_name);
  m_circle_item.setOpacity(m_circle_opacity);
  m_circle_item.setRotation(m_circle_rotation);

  m_circle_item.setWidth(m_circle_width);
  m_circle_item.setHeight(m_circle_width);
}

void TestCircleItem::toJsonTest() {
  QJsonObject expected_json;
  expected_json["x"] = QString::number(m_circle_x);
  expected_json["y"] = QString::number(m_circle_y);
  expected_json["width"] = QString::number(m_circle_width);
  expected_json["height"] = QString::number(m_circle_height);
  expected_json["item.color"] = m_circle_color;
  expected_json["item.name"] = m_circle_name;
  expected_json["item.rotation"] = QString::number(m_circle_rotation);
  expected_json["item.opacity"] = QString::number(m_circle_opacity);
  expected_json["item.file"] =
      "qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml";

  const auto circle_json = m_circle_item.toJson();

  QCOMPARE(circle_json, expected_json);
}

void TestCircleItem::paintTest() {
  QImage image(600, 400, QImage::Format::Format_ARGB32);
  QImage expected_image = image;
  image.fill("white");
  QPainter painter(&image);

  painter.save();
  painter.translate(m_circle_item.parentItem()->position());
  painter.setOpacity(m_circle_item.opacity());
  if (m_circle_item.rotation() != 0) {
    QPointF item_middle_point(m_circle_item.width() / 2.0,
                              m_circle_item.height() / 2.0);
    painter.translate(item_middle_point);
    painter.rotate(m_circle_item.rotation());
    painter.translate(-item_middle_point);
  }
  m_circle_item.paint(&painter);
  painter.restore();

  expected_image.load("://test_images/test_circle_image.png");

  QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestCircleItem)
#include "tst_circleitem.moc"
