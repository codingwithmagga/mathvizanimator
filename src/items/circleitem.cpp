#include "circleitem.h"

#include <QPainter>
#include <QPen>

CircleItem::CircleItem(QQuickItem *parent)
    : AbstractItem{parent}
{}

AbstractItem::ObjectContour CircleItem::getObjectContour() const
{
    //see https://stackoverflow.com/questions/14169234/the-relation-of-the-bezier-curve-and-ellipse

    const auto x = parentItem()->x();
    const auto y = parentItem()->y();

    const auto kappa = 4 * (sqrt(2) - 1) / 3;
    const auto ox = (width() / 2) * kappa;  // control point offset horizontal
    const auto oy = (height() / 2) * kappa; // control point offset vertical
    const auto xe = x + width();            // x-end
    const auto ye = y + height();           // y-end
    const auto xm = x + width() / 2;        // x-middle
    const auto ym = y + height() / 2;       // y-middle

    Point start_point_1{x, ym};
    Point cp11{x, ym - oy};
    Point cp12{xm - ox, y};
    Point end_point_1{xm, y};

    Point cp21{xm + ox, y};
    Point cp22{xe, ym - oy};
    Point end_point_2{xe, ym};

    Point cp31{xe, ym + oy};
    Point cp32{xm + ox, ye};
    Point end_point_3{xm, ye};

    Point cp41{xm - ox, ye};
    Point cp42{x, ym + oy};
    Point end_point_4{x, ym};

    BezierCurve firstCurve{start_point_1, cp11, cp12, end_point_1};
    BezierCurve secondCurve{end_point_1, cp21, cp22, end_point_2};
    BezierCurve thirdCurve{end_point_2, cp31, cp32, end_point_3};
    BezierCurve fourthCurve{end_point_3, cp41, cp42, end_point_4};

    ObjectContour object_contour;
    object_contour.contour = QVector<BezierCurve>{firstCurve, secondCurve, thirdCurve, fourthCurve};

    return object_contour;
}

void CircleItem::paint(QPainter *painter)
{
    painter->save();
    const auto pen_width = 4.0;

    QPen pen(color(), pen_width);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawEllipse(x() + pen_width / 2.0,
                         y() + pen_width / 2.0,
                         width() - pen_width,
                         height() - pen_width);
    painter->restore();
}
