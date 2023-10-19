#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "abstractitem.h"

class CircleItem : public AbstractItem {
    Q_OBJECT
    QML_ELEMENT

public:
    CircleItem(QQuickItem* parent = nullptr);

    void paint(QPainter* painter) override;

    EditableProperties editableProperties() const override;
};

#endif // CIRCLEITEM_H
