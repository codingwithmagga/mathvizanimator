#ifndef GEOMETRYITEM_H
#define GEOMETRYITEM_H

#include "abstractitem.h"

#include <QObject>

class GeometryItem : public AbstractItem {
    Q_OBJECT

    // There is already a filledColor property in QQuickPaintedItem so I call this "filledColor"
    Q_PROPERTY(QColor filledColor READ filledColor WRITE setFilledColor NOTIFY filledColorChanged)
    Q_PROPERTY(qreal filledOpacity READ filledOpacity WRITE setFilledOpacity NOTIFY filledOpacityChanged)

    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(qreal borderOpacity READ borderOpacity WRITE setBorderOpacity NOTIFY borderOpacityChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

  public:
    explicit GeometryItem(const QString& qml_file, BasicItem* parent = nullptr);

    QColor filledColor() const;
    void setFilledColor(const QColor& new_filled_color);

    qreal filledOpacity() const;
    void setFilledOpacity(const qreal& new_filled_opacity);

    QColor borderColor() const;
    void setBorderColor(const QColor& new_border_color);

    qreal borderOpacity() const;
    void setBorderOpacity(const qreal& new_border_opacity);

    qreal borderWidth() const;
    void setBorderWidth(const qreal& new_border_width);

    EditableProperties editableProperties() const override;

  signals:
    void filledColorChanged();
    void filledOpacityChanged();
    void borderColorChanged();
    void borderOpacityChanged();
    void borderWidthChanged();

  private:
    QColor m_filled_color = Qt::transparent;
    qreal m_filled_opacity = 1.0;

    QColor m_border_color;
    qreal m_border_opacity = 1.0;
    qreal m_border_width = 4.0;
};

#endif // GEOMETRYITEM_H
