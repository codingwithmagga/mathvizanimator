#include "textitem.h"

#include <QPainter>
#include <QPen>
#include <QSvgRenderer>

TextItem::TextItem(QQuickItem *parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/items/MVAText.qml", parent}
{}

void TextItem::paint(QPainter *painter)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing, true);

    QSvgRenderer renderer(m_svg_file.absoluteFilePath());

    const auto size = renderer.defaultSize();
    parentItem()->setWidth(size.width());
    parentItem()->setHeight(size.height());
    setWidth(size.width());
    setHeight(size.height());

    renderer.render(painter, QRect(QPoint(x(), y()), size));
    painter->restore();
}

QJsonObject TextItem::toJson() const
{
    return AbstractItem::toJson();
}
