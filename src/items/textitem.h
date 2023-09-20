#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QFileInfo>
#include <QObject>

#include "abstractitem.h"

class TextItem : public AbstractItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString latexSource READ getLatexSource WRITE setLatexSource)
    Q_PROPERTY(QFileInfo svgFile READ getSvgFile)

public:
    TextItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return m_object_type_; };

    QFileInfo getSvgFile() const { return m_svg_file; }

    void paint(QPainter *painter) override;

    QJsonObject toJson() const override;

    QString getLatexSource() const;
    void setLatexSource(const QString &newLatexSource);

private:
    ObjectType m_object_type_ = ObjectType::TEXT;

    QFileInfo m_svg_file = QFileInfo("./test.svg");
    QString m_latex_source;
};

#endif // TEXTITEM_H
