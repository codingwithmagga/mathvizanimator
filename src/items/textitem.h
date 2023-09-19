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

    QFileInfo getSvgFile() const { return m_svg_file; }

    void paint(QPainter *painter) override;

    QJsonObject toJson() const;

private:
    ObjectType m_object_type_ = ObjectType::TEXT;

    QFileInfo m_svg_file = QFileInfo("./test.svg");
};

#endif // TEXTITEM_H
