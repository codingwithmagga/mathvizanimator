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

class BasicItem;

/**
 * @brief An abstract class representing a visible object like a circle or a
 * rectangle.
 *
 * This is the base class for an visible object which can be added to the project. Derived classes have to implement the
 * QQuickPaintedItem::paint(QPainter* painter) function to specify how the object will be painted on the screen. In
 * addition the following functions should be implemented by a derived class if necessary:
 *
 * <ul>
 *   <li> QJsonObject AbstractItem::toJson() const;
 *   <li> EditableProperties AbstractItem::editableProperties() const;
 * </ul>
 *
 * The AbstractItem::paintItem(QPainter* painter) serves as some kind of wrapper function, which will be called when
 * rendering an image for the video. It applies properties to the items which otherwise is done by automatically by the
 * Qt Qml Framework.
 *
 * TODO: Qml file explain, needs BasicItem as parent class
 */
class AbstractItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString file MEMBER m_qml_file CONSTANT)

  public:
    struct EditableProperties {
        QStringList abstract_item_properties;
        QStringList basic_item_properties;
    };

    /**
     * @brief Constructor for MyClass. TODO
     *
     * Initializes the class with default values.
     */
    explicit AbstractItem(const QString& qml_file, BasicItem* parent = nullptr);

    /**
     * @brief Returns name of the item
     *
     * @return name of the item
     */
    QString name() const;
    void setName(const QString& name);

    virtual QJsonObject toJson() const;

    virtual EditableProperties editableProperties() const;

    // TODO(codingwithmagga): QVariantMap as return type seems to be a better
    // choice
    QList<QPair<QString, QVariant>> getItemProperties() const;
    QList<QPair<QString, QVariant>> getParentItemProperties() const;

    void paintItem(QPainter* painter);

  signals:
    void nameChanged(const QString& new_name);

  private:
    QList<QPair<QString, QVariant>> appendProperties(
        const auto obj, auto meta_object, const QStringList& allowedProperties) const;

    QString m_name;

    QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem) // GCOVR_EXCL_LINE

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_
