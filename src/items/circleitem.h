#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include "abstractitem.h"

class CircleItem : public AbstractItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    CircleItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return object_type_; };

    void paint(QPainter *painter) override;

    QJsonObject toJson() const;

private:
    ObjectType object_type_ = ObjectType::BEZIER;
};

#endif // CIRCLEITEM_H
