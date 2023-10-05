#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QDir>
#include <QFileInfo>
#include <QObject>

#include "abstractitem.h"

class TextItem : public AbstractItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString latexSource READ getLatexSource WRITE setLatexSource NOTIFY latexSourceChanged)
    Q_PROPERTY(QString svgFile READ getSvgFile WRITE setSvgFile NOTIFY svgFileChanged)

public:
    TextItem(QQuickItem *parent = nullptr);

    ObjectType getObjectType() const override { return m_object_type_; };

    QString getSvgFile() const { return m_svg_file.absoluteFilePath(); }
    void setSvgFile(const QFileInfo &newSvgFile);
    void setSvgFile(const QString &newSvgFile);

    void paint(QPainter *painter) override;

    QJsonObject toJson() const override;

    QString getLatexSource() const;
    void setLatexSource(const QString &newLatexSource);

signals:
    void latexSourceChanged(const QString &new_latex_source);
    void svgFileChanged(const QFileInfo &new_svg_file);

private:
    ObjectType m_object_type_ = ObjectType::TEXT;

    QFileInfo m_svg_file = QFileInfo(
        QDir::home().absoluteFilePath(".config/mathvizanimator/templates/test.svg"));
    QString m_latex_source;
};

#endif // TEXTITEM_H
