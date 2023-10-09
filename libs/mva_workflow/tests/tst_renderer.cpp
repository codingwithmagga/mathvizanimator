#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QSignalSpy>
#include <QTest>

#include "renderer.h"

#include "circleitem.h"
#include "rectangle.h"

class TestRenderer : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void render();

    void cleanupTestCase();

private:
    Renderer m_renderer;

    QList<AbstractItem *> m_item_list;
};

void TestRenderer::initTestCase()
{
    const qreal width_1 = 122;
    const qreal height_1 = width_1;
    const qreal width_2 = 111;
    const qreal height_2 = 82;

    auto parent_item_1 = new QQuickItem();
    parent_item_1->setX(55);
    parent_item_1->setY(65);
    parent_item_1->setHeight(width_1);
    parent_item_1->setWidth(height_1);

    auto parent_item_2 = new QQuickItem();
    parent_item_2->setX(455);
    parent_item_2->setY(265);
    parent_item_2->setHeight(width_2);
    parent_item_2->setWidth(height_2);

    auto circle = new CircleItem();
    circle->setWidth(width_1);
    circle->setHeight(height_1);
    circle->setParentItem(parent_item_1);
    circle->setColor("red");

    auto rect = new RectangleItem();
    rect->setWidth(width_2);
    rect->setHeight(height_2);
    rect->setParentItem(parent_item_2);
    rect->setColor("blue");

    qDebug() << circle->parentItem()->position();
    qDebug() << rect->parentItem()->position();

    m_item_list.push_back(circle);
    m_item_list.push_back(rect);
}

void TestRenderer::render()
{
    QSignalSpy spy(&m_renderer, &Renderer::finishedRendering);

    m_renderer.render(m_item_list);
    connect(&m_renderer, &Renderer::finishedRendering, [](const QFileInfo &file) {
        QVERIFY(file.exists());

        QMediaPlayer media_player;
        media_player.setSource(QUrl(file.absoluteFilePath()));

        QCOMPARE(media_player.metaData().value(QMediaMetaData::VideoFrameRate).toInt(), 24);
        QCOMPARE(media_player.metaData().value(QMediaMetaData::Duration).toLongLong(), 3000);
        QCOMPARE(media_player.metaData().value(QMediaMetaData::Resolution).toSize(),
                 QSize(600, 400));
    });

    QVERIFY(spy.wait(100000));
}

void TestRenderer::cleanupTestCase()
{
    qDeleteAll(m_item_list);
}

QTEST_MAIN(TestRenderer)
#include "tst_renderer.moc"
