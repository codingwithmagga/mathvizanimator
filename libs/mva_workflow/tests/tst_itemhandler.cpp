#include <QTest>

#include <QDirIterator>
#include <QQmlEngine>

#include "itemhandler.h"

class TestItemHandler : public QObject
{
    Q_OBJECT
private slots:

    void addItem();
    void addItemDuplicate();
    void addItemWithSameName();
    void addItemWithSameName2();
    void addIncompatibleItem();

    void checkItemData();
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
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

    ItemHandler itemhandler;
    QCOMPARE(itemhandler.numItems(), 0);

    itemhandler.addItem(circle);
    QCOMPARE(itemhandler.numItems(), 1);

    itemhandler.addItem(rect);
    QCOMPARE(itemhandler.numItems(), 2);
}

void TestItemHandler::addItemDuplicate()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(circle);

    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::addItemWithSameName()
{
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

void TestItemHandler::addItemWithSameName2()
{
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

void TestItemHandler::addIncompatibleItem()
{
    QSharedPointer<QQuickItem> quick_item(new QQuickItem);

    ItemHandler itemhandler;

    itemhandler.addItem(quick_item.data());

    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::checkItemData()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

    const int circleHeight = 123;
    const int rectHeight = 123;
    circle->setHeight(circleHeight);
    rect->setHeight(rectHeight);

    const auto circle_item = qvariant_cast<AbstractItem *>(circle->property("item"));
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
    QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem *>(), circle);
    QCOMPARE(circle_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem *>()->height(),
             circleHeight);
    QCOMPARE(circle_item_2->data(Qt::DisplayRole), "CircleItem");
    QCOMPARE(rect_item_1->data(Qt::DisplayRole), rect_item->name());
    QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem *>(), rect);
    QCOMPARE(rect_item_1->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem *>()->height(),
             rectHeight);
    QCOMPARE(rect_item_2->data(Qt::DisplayRole), "RectangleItem");
}

void TestItemHandler::getItems()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

    ItemHandler itemhandler;
    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    const auto item_list = itemhandler.items();

    QVERIFY(item_list.contains(circle));
    QVERIFY(item_list.contains(rect));
}

void TestItemHandler::removeItem()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.removeItem(circle);

    QCOMPARE(itemhandler.numItems(), 0);
}

void TestItemHandler::removeMultipleItems()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

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
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);

    itemhandler.removeItem(rect);
    QCOMPARE(itemhandler.numItems(), 1);
}

void TestItemHandler::clearHandler()
{
    auto circle = dynamic_cast<QQuickItem *>(m_circle_component.create());
    auto rect = dynamic_cast<QQuickItem *>(m_rect_component.create());

    ItemHandler itemhandler;

    itemhandler.addItem(circle);
    itemhandler.addItem(rect);

    itemhandler.clear();
    QCOMPARE(itemhandler.numItems(), 0);
}

QTEST_MAIN(TestItemHandler)
#include "tst_itemhandler.moc"
