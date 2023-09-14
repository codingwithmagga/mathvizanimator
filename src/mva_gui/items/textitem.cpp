#include "textitem.h"

#include <QPainter>
#include <QPen>
#include <QSvgRenderer>

TextItem::TextItem(QQuickItem *parent)
    : AbstractItem{parent}
{}

void TextItem::paint(QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);

    QSvgRenderer renderer(m_svg_file.absoluteFilePath());

    const auto size = renderer.defaultSize();
    parentItem()->setWidth(size.width());
    parentItem()->setHeight(size.height());
    setWidth(size.width());
    setHeight(size.height());

    renderer.render(painter, QRect(QPoint(x(), y()), size));
}
