/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_
#define LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_

#include <QColor>
#include <QFile>
#include <QJsonObject>
#include <QPair>
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

/**
 * @brief An abstract class representing a visible item like a circle or a
 * rectangle.
 *
 * Longer description...
 */
class AbstractItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString file MEMBER m_qml_file CONSTANT)

public:
    struct EditableProperties {
        QStringList abstract_item_properties;
        QStringList quick_item_properties;
    };

    /**
     * @brief Constructor for MyClass. TODO
     *
     * Initializes the class with default values.
     */
    explicit AbstractItem(const QString& qml_file, QQuickItem* parent = nullptr);

    QString name() const;
    void setName(const QString& name);

    QColor color() const;
    void setColor(const QColor& color);

    virtual QJsonObject toJson() const;

    virtual EditableProperties editableProperties() const;

    // TODO(codingwithmagga): QVariantMap as return type seems to be a better
    // choice
    QList<QPair<QString, QVariant>> getItemProperties() const;
    QList<QPair<QString, QVariant>> getParentItemProperties() const;

    void paintItem(QPainter* painter);

signals:
    void nameChanged(const QString& new_name);
    void colorChanged(const QColor& new_color);

private:
    QList<QPair<QString, QVariant>> appendProperties(
        const auto obj, auto meta_object,
        const QStringList& allowedProperties) const;

    QString m_name;
    QColor m_color;

    QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem) // GCOVR_EXCL_LINE

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_
