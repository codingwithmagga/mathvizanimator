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
    const qreal m_rect_opacity = 0.05;
    const qreal m_rect_rotation = -65;

    QQuickItem m_rect_parent_item;
    RectangleItem m_rect_item;
};

void TestRectangleItem::initTestCase()
{
    m_rect_parent_item.setX(m_rect_x);
    m_rect_parent_item.setY(m_rect_y);
    m_rect_parent_item.setWidth(m_rect_width);
    m_rect_parent_item.setHeight(m_rect_height);

    m_rect_item.setParentItem(&m_rect_parent_item);
    m_rect_item.setColor(QColor(m_rect_color));
    m_rect_item.setName(m_rect_name);
    m_rect_item.setOpacity(m_rect_opacity);
    m_rect_item.setRotation(m_rect_rotation);

    m_rect_item.setWidth(m_rect_width);
    m_rect_item.setHeight(m_rect_height);
}

void TestRectangleItem::toJsonTest()
{
    QJsonObject expected_json;
    expected_json["x"] = QString::number(m_rect_x);
    expected_json["y"] = QString::number(m_rect_y);
    expected_json["width"] = QString::number(m_rect_width);
    expected_json["height"] = QString::number(m_rect_height);
    expected_json["item.color"] = m_rect_color;
    expected_json["item.name"] = m_rect_name;
    expected_json["item.rotation"] = QString::number(m_rect_rotation);
    expected_json["item.opacity"] = QString::number(m_rect_opacity);
    expected_json["item.file"] = "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml";

    const auto rect_json = m_rect_item.toJson();

    QCOMPARE(rect_json, expected_json);
}

void TestRectangleItem::paintTest()
{
    QImage image(600, 400, QImage::Format::Format_ARGB32);
    QImage expected_image = image;
    image.fill("white");

    QPainter painter(&image);
    m_rect_item.paintItem(&painter);

    expected_image.load("://test_images/test_rectangle_image.png");
    QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestRectangleItem)
#include "tst_rectangleitem.moc"
