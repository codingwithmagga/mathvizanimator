#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "abstractitem.h"

class RectangleItem : public AbstractItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RectangleItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return object_type_; };
    ObjectContour getObjectContour() const override;

    ObjectStyle getObjectStyle() const override { return ObjectStyle{}; };
    void setObjectStyle(const ObjectStyle object_style) override{};

    QVector<AbstractItem *> getSubObjects() const override { return QVector<AbstractItem *>{}; }

    void paint(QPainter *painter) override;

private:
    ObjectType object_type_ = ObjectType::BEZIER;
};


#endif // RECTANGLE_H
