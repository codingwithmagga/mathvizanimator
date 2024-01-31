#include "basicitem.h"

#include "abstractitem.h"

BasicItem::BasicItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setHeight(100);
    setWidth(100);
}

void BasicItem::setAbstractItem(AbstractItem* const new_item)
{
    if (new_item == m_abstract_item) {
        return;
    }
    m_abstract_item = new_item;
    emit abstractItemChanged();
}

QString BasicItem::abstractItemName() const { return m_abstract_item->name(); }
