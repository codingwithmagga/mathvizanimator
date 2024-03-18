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

#include "circleitem.h"
#include "fade_in.h"
#include "fade_out.h"
#include "itemhandler.h"
#include "rectangle.h"

class TestItemHandler : public QObject {
    Q_OBJECT
  private slots:

    void addItem();
    void addItemDuplicate();
    void addItemWithSameName();
    void addItemWithSameName2();

    void addAnimation_data();
    void addAnimation();

    void addAnimations();

    void checkItemData();
    void checkItemProperties();

    void changeItemProperty();

    void scaleItemsPosX();
    void scaleItemsPosY();

    void scaleItemsWidth();
    void scaleItemsHeight();

    void getItems();

    void setTime();

    void removeItem();
    void removeCurrentItem();
    void removeMultipleItems();
    void removeNonExistingItem();

    void removeAnimation();

    void propertyModelFlags();

    void clearHandler();

  private:
    QList<BasicItem*> prepareItemHandler(ItemHandler* itemhandler, QList<QQmlComponent*> qml_components);

    BasicItem* extractQuickItemFromStandardItem(ItemModelItem* item) const;

    QQmlEngine m_engine;
    QQmlComponent m_circle_component
        = QQmlComponent(&m_engine, QUrl(QStringLiteral("qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml")));
    QQmlComponent m_rect_component
        = QQmlComponent(&m_engine, QUrl(QStringLiteral("qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml")));
};

void TestItemHandler::addItem()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

    ItemHandler itemhandler;
    QCOMPARE(itemhandler.numItems(), 0);

    itemhandler.addItem(circle);
    QCOMPARE(itemhandler.numItems(), 1);

    itemhandler.addItem(rect);
    QCOMPARE(itemhandler.numItems(), 2);
}

void TestItemHandler::addItemDuplicate()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(circle);

    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::addItemWithSameName()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto circle_1 = dynamic_cast<BasicItem*>(m_circle_component.create());

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

void TestItemHandler::addItemWithSameName2()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto circle_1 = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto circle_2 = dynamic_cast<BasicItem*>(m_circle_component.create());

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

void TestItemHandler::addAnimation_data()
{
    QTest::addColumn<QString>("animation_type");
    QTest::addColumn<qreal>("start_time");
    QTest::addColumn<qreal>("duration");

    QTest::newRow("FadeIn_default") << "FadeIn" << 0.0 << 1.0;
    QTest::newRow("FadeIn_long") << "FadeIn" << 0.0 << 3.2;
    QTest::newRow("FadeIn_late") << "FadeIn" << 3.0 << 1.2;
    QTest::newRow("FadeOut_default") << "FadeOut" << 0.0 << 1.0;
    QTest::newRow("FadeOut_long") << "FadeOut" << 0.0 << 3.2;
    QTest::newRow("FadeOut_late") << "FadeOut" << 3.0 << 1.2;
}

void TestItemHandler::addAnimation()
{
    QFETCH(QString, animation_type);
    QFETCH(qreal, start_time);
    QFETCH(qreal, duration);

    ItemHandler itemhandler;
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    itemhandler.addItem(circle);
    auto circle_item = circle->abstractItem();

    itemhandler.addAnimation(circle_item->name(), animation_type, start_time, duration);

    const auto item_observer_list = itemhandler.items();
    QCOMPARE(item_observer_list.size(), 1);

    const auto circle_item_observer = item_observer_list.first();
    const auto animation_list = circle_item_observer->animations();
    QCOMPARE(animation_list.size(), 1);

    const auto animation = animation_list.first();
    QCOMPARE(animation->metaObject()->className(), animation_type);
    QCOMPARE(animation->duration(), duration);
    QCOMPARE(animation->startTime(), start_time);
}

void TestItemHandler::addAnimations()
{

    ItemHandler itemhandler;
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());
    itemhandler.addItem(rect);
    const auto rect_item = rect->abstractItem();

    qreal duration = 1.2;
    qreal start_time = 2.5;

    auto fade_in = QSharedPointer<FadeIn>(new FadeIn());
    fade_in->setDuration(duration);
    auto fade_out = QSharedPointer<FadeOut>(new FadeOut());
    fade_out->setStartTime(start_time);
    QList<QSharedPointer<AbstractAnimation>> animation_list { fade_in, fade_out };

    itemhandler.addAnimations(rect_item->name(), animation_list);

    const auto item_observer_list = itemhandler.items();
    QCOMPARE(item_observer_list.size(), 1);

    const auto rect_item_observer = item_observer_list.first();
    const auto rect_animation_list = rect_item_observer->animations();
    QCOMPARE(rect_animation_list.size(), 2);

    const auto first_animation = rect_animation_list.first();
    QCOMPARE(first_animation->metaObject()->className(), "FadeIn");
    QCOMPARE(first_animation->duration(), duration);

    const auto second_animation = rect_animation_list.at(1);
    QCOMPARE(second_animation->metaObject()->className(), "FadeOut");
    QCOMPARE(second_animation->startTime(), start_time);
}

void TestItemHandler::checkItemData()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

    const qint32 circleHeight = 123;
    const qint32 rectHeight = 123;
    circle->setHeight(circleHeight);
    rect->setHeight(rectHeight);

    const auto circle_item = circle->abstractItem();
    const auto rect_item = rect->abstractItem();

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const auto model = itemhandler.model();
    const auto circle_item_1 = dynamic_cast<ItemModelItem*>(model->item(0));
    const auto circle_item_2 = model->item(0, 1);
    const auto rect_item_1 = dynamic_cast<ItemModelItem*>(model->item(1));
    const auto rect_item_2 = model->item(1, 1);

    QCOMPARE(circle_item_1->data(Qt::DisplayRole), circle_item->name());
    QCOMPARE(extractQuickItemFromStandardItem(circle_item_1), circle);
    QCOMPARE(extractQuickItemFromStandardItem(circle_item_1)->height(), circleHeight);
    QCOMPARE(circle_item_2->data(Qt::DisplayRole), "CircleItem");
    QCOMPARE(rect_item_1->data(Qt::DisplayRole), rect_item->name());
    QCOMPARE(extractQuickItemFromStandardItem(rect_item_1), rect);
    QCOMPARE(extractQuickItemFromStandardItem(rect_item_1)->height(), rectHeight);
    QCOMPARE(rect_item_2->data(Qt::DisplayRole), "RectangleItem");
}

void TestItemHandler::checkItemProperties()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

    const auto circle_item = circle->abstractItem();

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);
    itemhandler.setCurrentItem(circle_item->name());

    auto item_properties = circle_item->getItemProperties();
    item_properties.append(circle_item->getParentItemProperties());
    const auto propModel = itemhandler.propertyModel();

    QCOMPARE(propModel->rowCount(), item_properties.size());

    for (int i = 0; i < item_properties.size(); ++i) {
        const auto model_property = propModel->item(i)->data(Qt::DisplayRole).toString();
        const auto value = propModel->item(i, 1)->data(Qt::DisplayRole).toString();
        QCOMPARE(value, circle_item->property(model_property.toUtf8()).toString());
    }
}

void TestItemHandler::changeItemProperty()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    const auto circle_item = dynamic_cast<CircleItem*>(circle->abstractItem());

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.setCurrentItem(circle_item->name());

    const auto prop_model = itemhandler.propertyModel();
    const QString new_name("newName");
    const QColor new_color("green");

    QStandardItem* model_item_circle_name = nullptr;
    QStandardItem* model_item_circle_filled_color = nullptr;
    for (int i = 0; i < prop_model->rowCount(); ++i) {
        const auto model_property = prop_model->item(i)->data(Qt::DisplayRole).toString();
        if (model_property == "name") {
            prop_model->setData(prop_model->index(i, 1), new_name);
            model_item_circle_name = prop_model->item(i, 1);
        } else if (model_property == "filledColor") {
            prop_model->setData(prop_model->index(i, 1), new_color);
            model_item_circle_filled_color = prop_model->item(i, 1);
        }
    }

    QCOMPARE(circle_item->name(), new_name);
    QCOMPARE(circle_item->filledColor(), new_color);
    QVERIFY(model_item_circle_name);
    QVERIFY(model_item_circle_filled_color);
    QCOMPARE(model_item_circle_name->data(Qt::DisplayRole).toString(), new_name);
    QCOMPARE(model_item_circle_filled_color->data(Qt::DisplayRole).value<QColor>(), new_color);
}

void TestItemHandler::scaleItemsPosX()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

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
    itemhandler.scaleItems(ratio, 1.0);

    QCOMPARE(circle->x(), qRound(circle_x_old * ratio));
    QCOMPARE(circle->y(), circle_y_old);
    QCOMPARE(rect->x(), qRound(rect_x_old * ratio));
    QCOMPARE(rect->y(), rect_y_old);
}

void TestItemHandler::scaleItemsPosY()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

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
    itemhandler.scaleItems(1.0, ratio);

    QCOMPARE(circle->x(), circle_x_old);
    QCOMPARE(circle->y(), qRound(circle_y_old * ratio));
    QCOMPARE(rect->x(), rect_x_old);
    QCOMPARE(rect->y(), qRound(rect_y_old * ratio));
}

void TestItemHandler::scaleItemsWidth()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });

    const qreal circle_width_old = 222;
    const qreal circle_height_old = 133;
    const qreal rect_width_old = 116;
    const qreal rect_height_old = 332;
    const qreal circle_border_width_old = 20;
    const qreal rect_border_width_old = 5;

    items[0]->setSize(QSizeF(circle_width_old, circle_height_old));
    qobject_cast<CircleItem*>(items[0]->abstractItem())->setBorderWidth(circle_border_width_old);
    items[1]->setSize(QSizeF(rect_width_old, rect_height_old));
    qobject_cast<RectangleItem*>(items[1]->abstractItem())->setBorderWidth(rect_border_width_old);

    const qreal ratio = 1.2;
    const qreal avg_ratio = (ratio + 1.0) / 2.0;
    itemhandler.scaleItems(ratio, 1.0);

    QCOMPARE(items[0]->width(), qRound(circle_width_old * ratio));
    QCOMPARE(items[0]->height(), circle_height_old);
    QCOMPARE(qobject_cast<CircleItem*>(items[0]->abstractItem())->borderWidth(),
        qRound(avg_ratio * circle_border_width_old));
    QCOMPARE(items[1]->width(), qRound(rect_width_old * ratio));
    QCOMPARE(items[1]->height(), rect_height_old);
    QCOMPARE(qobject_cast<RectangleItem*>(items[1]->abstractItem())->borderWidth(),
        qRound(avg_ratio * rect_border_width_old));
}

void TestItemHandler::scaleItemsHeight()
{
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    auto rect = dynamic_cast<BasicItem*>(m_rect_component.create());

    const qreal circle_width_old = 202;
    const qreal circle_height_old = 276;
    const qreal rect_width_old = 454;
    const qreal rect_height_old = 238;
    const qreal circle_border_width_old = 50;
    const qreal rect_border_width_old = 35;

    circle->setSize(QSizeF(circle_width_old, circle_height_old));
    qobject_cast<CircleItem*>(circle->abstractItem())->setBorderWidth(circle_border_width_old);
    rect->setSize(QSizeF(rect_width_old, rect_height_old));
    qobject_cast<RectangleItem*>(rect->abstractItem())->setBorderWidth(rect_border_width_old);

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const qreal ratio = 0.4;
    const qreal avg_ratio = (ratio + 1.0) / 2.0;
    itemhandler.scaleItems(1.0, ratio);

    QCOMPARE(circle->height(), qRound(circle_height_old * ratio));
    QCOMPARE(circle->width(), circle_width_old);
    QCOMPARE(
        qobject_cast<CircleItem*>(circle->abstractItem())->borderWidth(), qRound(avg_ratio * circle_border_width_old));
    QCOMPARE(rect->height(), qRound(rect_height_old * ratio));
    QCOMPARE(rect->width(), rect_width_old);
    QCOMPARE(
        qobject_cast<RectangleItem*>(rect->abstractItem())->borderWidth(), qRound(avg_ratio * rect_border_width_old));
}

void TestItemHandler::getItems()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });

    const auto item_list = itemhandler.basicItems();

    QVERIFY(item_list.contains(items[0]));
    QVERIFY(item_list.contains(items[1]));
}

void TestItemHandler::setTime()
{
    ItemHandler itemhandler;
    const auto items
        = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });
    const auto circle_item = items[0]->abstractItem();
    const auto rect_item = items[1]->abstractItem();

    itemhandler.addAnimation(circle_item->name(), "FadeIn", 0.0, 1.0);
    itemhandler.addAnimation(rect_item->name(), "FadeOut", 0.0, 1.0);

    itemhandler.setTime(0.5);
    QCOMPARE(circle_item->opacity(), 0.5);
    QCOMPARE(rect_item->opacity(), 0.5);
}

void TestItemHandler::removeItem()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component });

    itemhandler.removeItem(items[0]);

    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeCurrentItem()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });
    const auto circle_item = items[0]->abstractItem();
    itemhandler.setCurrentItem(circle_item->name());

    itemhandler.removeCurrentItem();

    const auto itemhandler_items = itemhandler.basicItems();
    QCOMPARE(itemhandler_items.count(), 1);
    QCOMPARE(itemhandler_items[0], items[1]);
}

void TestItemHandler::removeMultipleItems()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });

    itemhandler.removeItem(items[1]);
    QCOMPARE(itemhandler.numItems(), 1);

    itemhandler.removeItem(items[0]);
    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeNonExistingItem()
{
    ItemHandler itemhandler;
    auto items = prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });

    itemhandler.removeItem(items[1]);
    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::removeAnimation()
{
    ItemHandler itemhandler;
    auto circle = dynamic_cast<BasicItem*>(m_circle_component.create());
    itemhandler.addItem(circle);
    const auto circle_item_name = circle->abstractItemName();
    itemhandler.setCurrentItem(circle_item_name);

    itemhandler.addAnimation(circle_item_name, "FadeIn", 0.0, 1.0);
    itemhandler.addAnimation(circle_item_name, "FadeOut", 1.1, 1.0);
    itemhandler.addAnimation(circle_item_name, "FadeIn", 2.2, 1.0);
    itemhandler.addAnimation(circle_item_name, "FadeOut", 3.3, 1.0);

    const auto item_observer_list = itemhandler.items();
    const auto item_observer = item_observer_list.at(0);
    QCOMPARE(item_observer->animations().size(), 4);

    int num_animations = 4;
    for (int i = 0; i < 4; ++i) {
        itemhandler.removeAnimation(0);
        num_animations--;
        QCOMPARE(item_observer->animations().size(), num_animations);
    }
}

void TestItemHandler::propertyModelFlags()
{
    PropertyModel prop_model;
    prop_model.appendRow({ new QStandardItem, new QStandardItem, new QStandardItem });
    const auto index_column_0 = prop_model.index(0, 0);
    const auto index_column_1 = prop_model.index(0, 1);
    const auto index_column_2 = prop_model.index(0, 2);

    const auto flags_column_0 = prop_model.flags(index_column_0);
    const auto flags_column_1 = prop_model.flags(index_column_1);
    const auto flags_column_2 = prop_model.flags(index_column_2);

    QVERIFY(!flags_column_0.testAnyFlag(Qt::ItemIsEditable));
    QVERIFY(flags_column_1.testAnyFlag(Qt::ItemIsEditable));
    QVERIFY(!flags_column_2.testAnyFlag(Qt::ItemIsEditable));
}

void TestItemHandler::clearHandler()
{
    ItemHandler itemhandler;
    prepareItemHandler(&itemhandler, QList<QQmlComponent*> { &m_circle_component, &m_rect_component });

    itemhandler.clear();
    QCOMPARE(itemhandler.numItems(), 0);
}

QList<BasicItem*> TestItemHandler::prepareItemHandler(ItemHandler* itemhandler, QList<QQmlComponent*> qml_components)
{
    QList<BasicItem*> items;

    for (auto& component : qml_components) {
        items.push_back(dynamic_cast<BasicItem*>(component->create()));
    }

    for (const auto& item : items) {
        itemhandler->addItem(item);
    }

    return items;
}

BasicItem* TestItemHandler::extractQuickItemFromStandardItem(ItemModelItem* item) const
{
    return item->itemObserver()->item();
}

QTEST_MAIN(TestItemHandler)
#include "tst_itemhandler.moc"
