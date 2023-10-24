#include <QTest>

#include <QDirIterator>
#include <QQmlEngine>

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

    void recalcItemsPosX();
    void recalcItemsPosY();

    void recalcItemsWidth();
    void recalcItemsHeight();

    void getItems();

    void removeItem();
    void removeMultipleItems();
    void removeNonExistingItem();

    void clearHandler();

private:
    QQmlEngine m_engine;
    QQmlComponent m_circle_component
        = QQmlComponent(&m_engine,
            QUrl(QStringLiteral("qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml")));
    QQmlComponent m_rect_component
        = QQmlComponent(&m_engine,
            QUrl(QStringLiteral("qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml")));
};

void TestItemHandler::addItem()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    ItemHandler itemhandler;
    QCOMPARE(itemhandler.numItems(), 0);

    itemhandler.addItem(circle);
    QCOMPARE(itemhandler.numItems(), 1);

    itemhandler.addItem(rect);
    QCOMPARE(itemhandler.numItems(), 2);
}

void TestItemHandler::addItemDuplicate()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(circle);

    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::addItemWithSameName()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto circle_1 = dynamic_cast<QQuickItem*>(m_circle_component.create());

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
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto circle_1 = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto circle_2 = dynamic_cast<QQuickItem*>(m_circle_component.create());

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

void TestItemHandler::addIncompatibleItem()
{
    QSharedPointer<QQuickItem> quick_item(new QQuickItem);

    ItemHandler itemhandler;

    itemhandler.addItem(quick_item.data());

    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::checkItemData()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const int circleHeight = 123;
    const int rectHeight = 123;
    circle->setHeight(circleHeight);
    rect->setHeight(rectHeight);

    const auto circle_item = qvariant_cast<AbstractItem*>(circle->property("item"));
    const auto rect_item = qvariant_cast<AbstractItem*>(rect->property("item"));

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const auto model = itemhandler.model();
    const auto circle_item_1 = model->item(0);
    const auto circle_item_2 = model->item(0, 1);
    const auto rect_item_1 = model->item(1);
    const auto rect_item_2 = model->item(1, 1);

    QCOMPARE(circle_item_1->data(Qt::DisplayRole), circle_item->name());
    QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem*>(), circle);
    QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem*>()->height(),
        circleHeight);
    QCOMPARE(circle_item_2->data(Qt::DisplayRole), "CircleItem");
    QCOMPARE(rect_item_1->data(Qt::DisplayRole), rect_item->name());
    QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem*>(), rect);
    QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem*>()->height(),
        rectHeight);
    QCOMPARE(rect_item_2->data(Qt::DisplayRole), "RectangleItem");
}

void TestItemHandler::checkItemProperties()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const auto circle_item = qvariant_cast<AbstractItem*>(circle->property("item"));

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

void TestItemHandler::changeItemProperty()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    const auto circle_item = qvariant_cast<AbstractItem*>(circle->property("item"));

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

void TestItemHandler::recalcItemsPosX()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const qreal circle_x_old = 200;
    const qreal circle_y_old = 100;
    const qreal rect_x_old = 154;
    const qreal rect_y_old = 432;

    circle->setX(circle_x_old);
    circle->setY(circle_y_old);
    rect->setX(rect_x_old);
    rect->setY(rect_y_old);

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const qreal ratio = 0.65;
    itemhandler.recalcItemsX(ratio);

    QCOMPARE(circle->x(), qRound(circle_x_old * ratio));
    QCOMPARE(circle->y(), circle_y_old);
    QCOMPARE(rect->x(), qRound(rect_x_old * ratio));
    QCOMPARE(rect->y(), rect_y_old);
}

void TestItemHandler::recalcItemsPosY()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const qreal circle_x_old = 200;
    const qreal circle_y_old = 100;
    const qreal rect_x_old = 154;
    const qreal rect_y_old = 432;

    circle->setX(circle_x_old);
    circle->setY(circle_y_old);
    rect->setX(rect_x_old);
    rect->setY(rect_y_old);

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const qreal ratio = 1.4;
    itemhandler.recalcItemsY(ratio);

    QCOMPARE(circle->x(), circle_x_old);
    QCOMPARE(circle->y(), qRound(circle_y_old * ratio));
    QCOMPARE(rect->x(), rect_x_old);
    QCOMPARE(rect->y(), qRound(rect_y_old * ratio));
}

void TestItemHandler::recalcItemsWidth()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const qreal circle_width_old = 200;
    const qreal circle_height_old = 130;
    const qreal rect_width_old = 154;
    const qreal rect_height_old = 432;

    circle->setWidth(circle_width_old);
    circle->setHeight(circle_height_old);
    rect->setWidth(rect_width_old);
    rect->setHeight(rect_height_old);

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const qreal ratio = 1.2;
    itemhandler.recalcItemsWidth(ratio);

    QCOMPARE(circle->width(), qRound(circle_width_old * ratio));
    QCOMPARE(circle->height(), circle_height_old);
    QCOMPARE(rect->width(), qRound(rect_width_old * ratio));
    QCOMPARE(rect->height(), rect_height_old);
}

void TestItemHandler::recalcItemsHeight()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    const qreal circle_width_old = 200;
    const qreal circle_height_old = 130;
    const qreal rect_width_old = 154;
    const qreal rect_height_old = 432;

    circle->setWidth(circle_width_old);
    circle->setHeight(circle_height_old);
    rect->setWidth(rect_width_old);
    rect->setHeight(rect_height_old);

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const qreal ratio = 0.4;
    itemhandler.recalcItemsHeight(ratio);

    QCOMPARE(circle->height(), qRound(circle_height_old * ratio));
    QCOMPARE(circle->width(), circle_width_old);
    QCOMPARE(rect->height(), qRound(rect_height_old * ratio));
    QCOMPARE(rect->width(), rect_width_old);
}

void TestItemHandler::getItems()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const auto item_list = itemhandler.items();

    QVERIFY(item_list.contains(circle));
    QVERIFY(item_list.contains(rect));
}

void TestItemHandler::removeItem()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.removeItem(circle);

    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeMultipleItems()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    itemhandler.removeItem(rect);
    QCOMPARE(itemhandler.numItems(), 1);

    itemhandler.removeItem(circle);
    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeNonExistingItem()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);

    itemhandler.removeItem(rect);
    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::clearHandler()
{
    auto circle = dynamic_cast<QQuickItem*>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem*>(m_rect_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    itemhandler.clear();
    QCOMPARE(itemhandler.numItems(), 0);
}

QTEST_MAIN(TestItemHandler)
#include "tst_itemhandler.moc"
