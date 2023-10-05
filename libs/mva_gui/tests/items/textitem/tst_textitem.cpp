#include <QPainter>
#include <QTest>

#include "textitem.h"

class TestTextItem : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void toJsonTest();
    void latexRenderTest();
    void paintTest();

private:
    const qreal m_test_x = 46;
    const qreal m_test_y = 98;
    const qreal m_test_width = 124;
    const qreal m_test_height = 276;

    const QString m_test_color = "#0000ff";
    const QString m_test_name = "test_text_name";

    QQuickItem m_test_parent_item;
    TextItem m_test_text;
};

void TestTextItem::initTestCase()
{
    m_test_parent_item.setX(m_test_x);
    m_test_parent_item.setY(m_test_y);
    m_test_parent_item.setWidth(m_test_width);
    m_test_parent_item.setHeight(m_test_height);

    m_test_text.setParentItem(&m_test_parent_item);
    m_test_text.setColor(QColor(m_test_color));
    m_test_text.setName(m_test_name);
}

void TestTextItem::toJsonTest()
{
    QJsonObject expected_json;
    expected_json["x"] = m_test_x;
    expected_json["y"] = m_test_y;
    expected_json["width"] = m_test_width;
    expected_json["height"] = m_test_height;
    expected_json["item.color"] = m_test_color;
    expected_json["item.name"] = m_test_name;
    expected_json["item.file"] = "qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml";
    expected_json["item.latexSource"] = "";
    expected_json["item.svgFile"] = m_test_text.getSvgFile();

    const auto rect_json = m_test_text.toJson();

    QCOMPARE(rect_json, expected_json);
}

void TestTextItem::latexRenderTest()
{
    const QString test_latex = R"($\theta_1 = \delta + \epsilon$)";
    m_test_text.setLatexSource(test_latex);

    QCOMPARE(m_test_text.getLatexSource(), test_latex);
    QCOMPARE(m_test_text.getSvgFile(),
             QDir::current().absoluteFilePath("0f35255d1355d2c32390101bf57ff4d0.svg"));
}

void TestTextItem::paintTest()
{
    QImage image(600, 400, QImage::Format::Format_ARGB32);
    QImage expected_image = image;
    image.fill("white");
    QPainter painter(&image);

    painter.save();
    painter.translate(m_test_text.parentItem()->position());
    m_test_text.paint(&painter);
    painter.restore();

    expected_image.load("textitem/test_text.png");

    QCOMPARE(image, expected_image);
}

QTEST_MAIN(TestTextItem)
#include "tst_textitem.moc"
