#include <QTest>

#include "circleitem.h"

class TestCircleItem : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void toJsonTest();

private:
    const qreal m_test_x = 46;
    const qreal m_test_y = 98;
    const qreal m_test_width = 124;
    const qreal m_test_height = 276;

    const QString m_test_color = "#0000ff";
    const QString m_test_name = "test_circle_name";

    QQuickItem m_test_parent_item;
    CircleItem m_test_circle;
};

void TestCircleItem::initTestCase()
{
    m_test_parent_item.setX(m_test_x);
    m_test_parent_item.setY(m_test_y);
    m_test_parent_item.setWidth(m_test_width);
    m_test_parent_item.setHeight(m_test_height);

    m_test_circle.setParentItem(&m_test_parent_item);
    m_test_circle.setColor(QColor(m_test_color));
    m_test_circle.setName(m_test_name);
}

void TestCircleItem::toJsonTest()
{
    QJsonObject expected_json;
    expected_json["x"] = m_test_x;
    expected_json["y"] = m_test_y;
    expected_json["width"] = m_test_width;
    expected_json["height"] = m_test_height;
    expected_json["item.color"] = m_test_color;
    expected_json["item.name"] = m_test_name;
    expected_json["item.file"] = "qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml";

    const auto circle_json = m_test_circle.toJson();

    QCOMPARE(circle_json, expected_json);
}

QTEST_MAIN(TestCircleItem)
#include "tst_circleitem.moc"
