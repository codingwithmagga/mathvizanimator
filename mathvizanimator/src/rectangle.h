#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "abstractobject.h"

class RectangleItem : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RectangleItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return object_type_; };
    ObjectContour getObjectContour() const override;

    ObjectStyle getObjectStyle() const override { return ObjectStyle{}; };
    void setObjectStyle(const ObjectStyle object_style) override{};

    QVector<AbstractObject *> getSubObjects() const override
    {
        return QVector<AbstractObject *>{};
    };

    void paint(QPainter *painter) override;

public slots:
    void onPressed(const qreal &mouse_x, const qreal &mouse_y);
    void onPosChanged(const qreal &mouse_x, const qreal &mouse_y);

protected:
    void componentComplete() override;

private:
    ObjectType object_type_ = ObjectType::BEZIER;

    qreal start_x_ = 0;
    qreal start_y_ = 0;
};

Q_DECLARE_METATYPE(RectangleItem)

#endif // RECTANGLE_H
