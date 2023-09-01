#ifndef ABSTRACTOBJECT_H
#define ABSTRACTOBJECT_H

#include <QColor>
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

class AbstractObject : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    enum ObjectType { BEZIER, TEXT };

    struct ObjectStyle
    {
        int lineWidth;
    };

    struct Point
    {
        double x;
        double y;
    };

    struct BezierCurve
    {
        Point start;
        Point first_control_point;
        Point second_control_point;
        Point end;
    };

    struct ObjectContour
    {
        QVector<BezierCurve> contour;
    };

    AbstractObject(QQuickItem *parent = nullptr);

    virtual ObjectType getObjectType() const = 0;
    virtual ObjectContour getObjectContour() const = 0;

    virtual ObjectStyle getObjectStyle() const = 0;
    virtual void setObjectStyle(const ObjectStyle object_style) = 0;

    virtual QVector<AbstractObject *> getSubObjects() const = 0;

    QString name() const;
    void setName(const QString &name);

    QColor color() const;
    void setColor(const QColor &color);

    qreal mousearea_width() const;
    void setMousearea_width(qreal newMousearea_width);

    qreal mousearea_height() const;
    void setMousearea_height(qreal newMousearea_height);

private:
    qreal mousearea_width_ = 600;
    qreal mousearea_height_ = 400;

    QString name_;
    QColor color_;
};

Q_DECLARE_METATYPE(AbstractObject)

#endif // ABSTRACTOBJECT_H
