#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QFileInfo>
#include <QObject>

#include "abstractitem.h"

class TextItem : public AbstractItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    TextItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return m_object_type_; };
    ObjectContour getObjectContour() const override { return ObjectContour{}; }
    ObjectStyle getObjectStyle() const override { return ObjectStyle{}; };
    void setObjectStyle(const ObjectStyle object_style) override{};
    QVector<AbstractItem *> getSubObjects() const override { return QVector<AbstractItem *>{}; }

    QFileInfo getSvgFile() const { return m_svg_file; }

    void paint(QPainter *painter) override;

private:
    ObjectType m_object_type_ = ObjectType::TEXT;

    QFileInfo m_svg_file = QFileInfo("./test.svg");
};

#endif // TEXTITEM_H
