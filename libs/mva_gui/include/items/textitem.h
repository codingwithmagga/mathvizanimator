#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QDir>
#include <QFileInfo>
#include <QObject>

#include "abstractitem.h"

class TextItem : public AbstractItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString latexSource READ getLatexSource WRITE setLatexSource NOTIFY latexSourceChanged)
    Q_PROPERTY(QString svgFile READ getSvgFile WRITE setSvgFile NOTIFY svgFileChanged)
    Q_PROPERTY(qreal scaleText READ getScaleText WRITE setScaleText NOTIFY scaleTextChanged)

public:
    TextItem(QQuickItem* parent = nullptr);

    QString getSvgFile() const { return m_svg_file.absoluteFilePath(); }
    void setSvgFile(const QFileInfo& newSvgFile);
    void setSvgFile(const QString& newSvgFile);

    void paint(QPainter* painter) override;

    QJsonObject toJson() const override;

    QString getLatexSource() const;
    void setLatexSource(const QString& newLatexSource);

    qreal getScaleText() const;
    void setScaleText(qreal newScaleText);

    EditableProperties editableProperties() const override;

signals:
    void latexSourceChanged(const QString& new_latex_source);
    void svgFileChanged(const QFileInfo& new_svg_file);
    void scaleTextChanged(const qreal new_scale_text);

private:
    QFileInfo m_svg_file = QFileInfo("://templates/test.svg");
    QString m_latex_source;
    qreal m_scale_text = 1.0;
};

#endif // TEXTITEM_H
