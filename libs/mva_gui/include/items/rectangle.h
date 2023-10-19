#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "abstractitem.h"

class RectangleItem : public AbstractItem {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RectangleItem(QQuickItem* parent = nullptr);

    void paint(QPainter* painter) override;

    EditableProperties editableProperties() const override;
};

#endif // RECTANGLE_H
