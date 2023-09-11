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
    ObjectContour getObjectContour() const override;
    ObjectStyle getObjectStyle() const override { return ObjectStyle{}; };
    void setObjectStyle(const ObjectStyle object_style) override{};
    QVector<AbstractItem *> getSubObjects() const override { return QVector<AbstractItem *>{}; }

    void paint(QPainter *painter) override;

private:
    ObjectType object_type_ = ObjectType::BEZIER;
};

#endif // CIRCLEITEM_H
