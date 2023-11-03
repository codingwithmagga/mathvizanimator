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

#include <QDirIterator>
#include <QQmlEngine>
#include <QTest>

#include "itemhandler.h"

class TestItemHandler : public QObject {
  Q_OBJECT
 private slots:

  void addItem();
  void addItemDuplicate();
  void addItemWithSameName();
  void addItemWithSameName2();
  void addIncompatibleItem();

  void checkItemData();
  void checkItemProperties();

  void changeItemProperty();

  void scaleItemsPosX();
  void scaleItemsPosY();

  void scaleItemsWidth();
  void scaleItemsHeight();

  void getItems();

  void removeItem();
  void removeMultipleItems();
  void removeNonExistingItem();

  void clearHandler();

 private:
  QList<QQuickItem *> prepareItemHandler(ItemHandler *itemhandler,
                                         QList<QQmlComponent *> qml_components);

  QQmlEngine m_engine;
  QQmlComponent m_circle_component = QQmlComponent(
      &m_engine,
      QUrl(QStringLiteral("qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml")));
  QQmlComponent m_rect_component = QQmlComponent(
      &m_engine, QUrl(QStringLiteral(
                     "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml")));
};

void TestItemHandler::addItem() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  ItemHandler itemhandler;
  QCOMPARE(itemhandler.numItems(), 0);

  itemhandler.addItem(circle);
  QCOMPARE(itemhandler.numItems(), 1);

  itemhandler.addItem(rect);
  QCOMPARE(itemhandler.numItems(), 2);
}

void TestItemHandler::addItemDuplicate() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());

  ItemHandler itemhandler;

  itemhandler.addItem(circle);
  itemhandler.addItem(circle);

  QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::addItemWithSameName() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto circle_1 = dynamic_cast<QQuickItem *>(m_circle_component.create());

  ItemHandler itemhandler;

  itemhandler.addItem(circle);
  itemhandler.addItem(circle_1);

  QCOMPARE(itemhandler.numItems(), 2);

  const auto model = itemhandler.model();
  const auto circle_name = model->item(0)->text();
  const auto new_circle_1_name = model->item(1)->text();

  QCOMPARE_NE(circle_name, new_circle_1_name);
  QCOMPARE(new_circle_1_name, "circle_1");
}

void TestItemHandler::addItemWithSameName2() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto circle_1 = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto circle_2 = dynamic_cast<QQuickItem *>(m_circle_component.create());

  ItemHandler itemhandler;

  itemhandler.addItem(circle);
  itemhandler.addItem(circle_1);
  itemhandler.addItem(circle_2);

  QCOMPARE(itemhandler.numItems(), 3);

  const auto model = itemhandler.model();
  const auto circle_name = model->item(0)->text();
  const auto circle_1_name = model->item(1)->text();
  const auto new_circle_2_name = model->item(2)->text();

  QCOMPARE_NE(circle_name, new_circle_2_name);
  QCOMPARE_NE(circle_1_name, new_circle_2_name);
  QCOMPARE(new_circle_2_name, "circle_2");
}

void TestItemHandler::addIncompatibleItem() {
  QSharedPointer<QQuickItem> quick_item(new QQuickItem);

  ItemHandler itemhandler;

  itemhandler.addItem(quick_item.data());

  QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::checkItemData() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  const int circleHeight = 123;
  const int rectHeight = 123;
  circle->setHeight(circleHeight);
  rect->setHeight(rectHeight);

  const auto circle_item =
      qvariant_cast<AbstractItem *>(circle->property("item"));
  const auto rect_item = qvariant_cast<AbstractItem *>(rect->property("item"));

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.addItem(rect);

  const auto model = itemhandler.model();
  const auto circle_item_1 = model->item(0);
  const auto circle_item_2 = model->item(0, 1);
  const auto rect_item_1 = model->item(1);
  const auto rect_item_2 = model->item(1, 1);

  QCOMPARE(circle_item_1->data(Qt::DisplayRole), circle_item->name());
  QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM)
               .value<QQuickItem *>(),
           circle);
  QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM)
               .value<QQuickItem *>()
               ->height(),
           circleHeight);
  QCOMPARE(circle_item_2->data(Qt::DisplayRole), "CircleItem");
  QCOMPARE(rect_item_1->data(Qt::DisplayRole), rect_item->name());
  QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM)
               .value<QQuickItem *>(),
           rect);
  QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM)
               .value<QQuickItem *>()
               ->height(),
           rectHeight);
  QCOMPARE(rect_item_2->data(Qt::DisplayRole), "RectangleItem");
}

void TestItemHandler::checkItemProperties() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  const auto circle_item =
      qvariant_cast<AbstractItem *>(circle->property("item"));

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.addItem(rect);
  itemhandler.setCurrentRow(0);

  const auto propModel = itemhandler.propertyModel();

  QVERIFY(propModel->rowCount() > 0);

  const auto prop_1_type = propModel->item(0);
  const auto prop_1_val = propModel->item(0, 1);

  QCOMPARE(prop_1_type->data(Qt::DisplayRole).toString(), "name");
  QCOMPARE(prop_1_val->data(Qt::DisplayRole).toString(), circle_item->name());
}

void TestItemHandler::changeItemProperty() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  const auto circle_item =
      qvariant_cast<AbstractItem *>(circle->property("item"));

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.setCurrentRow(0);

  const auto prop_model = itemhandler.propertyModel();
  const QString new_name("newName");
  const QColor new_color("green");
  prop_model->setData(prop_model->index(0, 1), new_name);
  prop_model->setData(prop_model->index(1, 1), new_color);

  const auto item_model = itemhandler.model();
  const auto item_model_circle = item_model->item(0);
  QCOMPARE(circle_item->name(), new_name);
  QCOMPARE(circle_item->color(), new_color);
  QCOMPARE(item_model_circle->data(Qt::DisplayRole).toString(), new_name);
}

void TestItemHandler::scaleItemsPosX() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  const qreal circle_x_old = 220;
  const qreal circle_y_old = 111;
  const qreal rect_x_old = 254;
  const qreal rect_y_old = 432;

  circle->setPosition(QPointF(circle_x_old, circle_y_old));
  rect->setPosition(QPointF(rect_x_old, rect_y_old));

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.addItem(rect);

  const qreal ratio = 0.65;
  itemhandler.scaleItemsX(ratio);

  QCOMPARE(circle->x(), qRound(circle_x_old * ratio));
  QCOMPARE(circle->y(), circle_y_old);
  QCOMPARE(rect->x(), qRound(rect_x_old * ratio));
  QCOMPARE(rect->y(), rect_y_old);
}

void TestItemHandler::scaleItemsPosY() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.addItem(rect);

  const qreal circle_x_old = 201;
  const qreal circle_y_old = 152;
  const qreal rect_x_old = 158;
  const qreal rect_y_old = 472;

  circle->setPosition(QPointF(circle_x_old, circle_y_old));
  rect->setPosition(QPointF(rect_x_old, rect_y_old));

  const qreal ratio = 1.4;
  itemhandler.scaleItemsY(ratio);

  QCOMPARE(circle->x(), circle_x_old);
  QCOMPARE(circle->y(), qRound(circle_y_old * ratio));
  QCOMPARE(rect->x(), rect_x_old);
  QCOMPARE(rect->y(), qRound(rect_y_old * ratio));
}

void TestItemHandler::scaleItemsWidth() {
  ItemHandler itemhandler;
  auto items = prepareItemHandler(
      &itemhandler,
      QList<QQmlComponent *>{&m_circle_component, &m_rect_component});

  const qreal circle_width_old = 222;
  const qreal circle_height_old = 133;
  const qreal rect_width_old = 116;
  const qreal rect_height_old = 332;

  items[0]->setSize(QSizeF(circle_width_old, circle_height_old));
  items[1]->setSize(QSizeF(rect_width_old, rect_height_old));

  const qreal ratio = 1.2;
  itemhandler.scaleItemsWidth(ratio);

  QCOMPARE(items[0]->width(), qRound(circle_width_old * ratio));
  QCOMPARE(items[0]->height(), circle_height_old);
  QCOMPARE(items[1]->width(), qRound(rect_width_old * ratio));
  QCOMPARE(items[1]->height(), rect_height_old);
}

void TestItemHandler::scaleItemsHeight() {
  auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
  auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

  const qreal circle_width_old = 202;
  const qreal circle_height_old = 276;
  const qreal rect_width_old = 454;
  const qreal rect_height_old = 238;

  circle->setSize(QSizeF(circle_width_old, circle_height_old));
  rect->setSize(QSizeF(rect_width_old, rect_height_old));

  ItemHandler itemhandler;
  itemhandler.addItem(circle);
  itemhandler.addItem(rect);

  const qreal ratio = 0.4;
  itemhandler.scaleItemsHeight(ratio);

  QCOMPARE(circle->height(), qRound(circle_height_old * ratio));
  QCOMPARE(circle->width(), circle_width_old);
  QCOMPARE(rect->height(), qRound(rect_height_old * ratio));
  QCOMPARE(rect->width(), rect_width_old);
}

void TestItemHandler::getItems() {
  ItemHandler itemhandler;
  auto items = prepareItemHandler(
      &itemhandler,
      QList<QQmlComponent *>{&m_circle_component, &m_rect_component});

  const auto item_list = itemhandler.items();

  QVERIFY(item_list.contains(items[0]));
  QVERIFY(item_list.contains(items[1]));
}

void TestItemHandler::removeItem() {
  ItemHandler itemhandler;
  auto items = prepareItemHandler(&itemhandler,
                                  QList<QQmlComponent *>{&m_circle_component});

  itemhandler.removeItem(items[0]);

  QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeMultipleItems() {
  ItemHandler itemhandler;
  auto items = prepareItemHandler(
      &itemhandler,
      QList<QQmlComponent *>{&m_circle_component, &m_rect_component});

  itemhandler.removeItem(items[1]);
  QCOMPARE(itemhandler.numItems(), 1);

  itemhandler.removeItem(items[0]);
  QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeNonExistingItem() {
  ItemHandler itemhandler;
  auto items = prepareItemHandler(
      &itemhandler,
      QList<QQmlComponent *>{&m_circle_component, &m_rect_component});

  itemhandler.removeItem(items[1]);
  QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::clearHandler() {
  ItemHandler itemhandler;
  prepareItemHandler(&itemhandler, QList<QQmlComponent *>{&m_circle_component,
                                                          &m_rect_component});

  itemhandler.clear();
  QCOMPARE(itemhandler.numItems(), 0);
}

QList<QQuickItem *> TestItemHandler::prepareItemHandler(
    ItemHandler *itemhandler, QList<QQmlComponent *> qml_components) {
  QList<QQuickItem *> items;

  for (auto &component : qml_components) {
    items.push_back(dynamic_cast<QQuickItem *>(component->create()));
  }

  for (const auto &item : items) {
    itemhandler->addItem(item);
  }

  return items;
}

QTEST_MAIN(TestItemHandler)
#include "tst_itemhandler.moc"
