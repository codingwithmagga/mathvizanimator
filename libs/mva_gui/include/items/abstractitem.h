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
#include <QVariantMap>
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

class BasicItem;

/**
 * @brief Map which stores key (QString) value (QVariant) pairs.
 *
 * Derived from QVariantMap which is typedef for QMap<QString, QVariant>. Stores the properties of an item with its
 * values. The function stringValue converts the QVariant value of the given property to a QString.
 */
class PropertyMap : public QVariantMap {
  public:
    /**
     * @brief Returns the value to the given property as QString.
     *
     * An empty QString will be returned when the value can't be converted or the given property does not exist in the
     * Map.
     *
     * @param property The property which value should be returned.
     */
    inline QString stringValue(const QString& property) const { return value(property).toString(); }
};

/**
 * @brief An abstract class representing a visible item like a circle or a
 * rectangle.
 *
 * Add editable properties to the list
 */
class AbstractItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString file MEMBER m_qml_file CONSTANT)

  public:
    /**
     * @brief Constructor for MyClass. TODO
     *
     * Initializes the class with default values.
     */
    explicit AbstractItem(const QString& qml_file, BasicItem* parent = nullptr);

    QString name() const;
    void setName(const QString& name);

    virtual QJsonObject toJson() const;

    /**
     * @brief Returns the properties which can be edited by the user in a QStringList
     *
     * When you derive an item from AbstractItem (directly or via multiple inheritance) and your item has a property
     * which should be editable by the user, you have to override this function and return your own properties. Don't
     * forget to add the properties of your base class. Take a look at the GeometryItem::editableProperties() source
     * code for an example.
     *
     * @return Returns the editable properties as QStrings
     */
    virtual inline QStringList editableProperties() const { return { "name", "opacity", "rotation" }; }

    /**
     * @brief Returns the parents properties which can be edited by the user in a QStringList
     *
     * The position (x,y) and the width and height of an AbstractItem are determined by the parent which is an object of
     * type BasicItem.The position (x,y) of the AbstractItem is relative to the parent, the BasicItem. So changing these
     * values by the user is not intended. Since the AbstractItem should always fill the whole parent (search for
     * "anchors.fill: parent" in the item qml files, for example CircleItem.qml) it is also reasonable to change width
     * and height of the BasicItem by the user and not the widht and height of the AbstractItem.
     *
     * @return Returns the editable properties as QStrings
     */
    virtual inline QStringList editablePropertiesParent() const { return { "width", "height", "x", "y" }; }

    PropertyMap getItemProperties() const;
    PropertyMap getParentItemProperties() const;

    /**
     * @brief Short explanation
     *
     * TODO
     */
    void paintItem(QPainter* painter);

  signals:
    void nameChanged(const QString& new_name);

  private:
    PropertyMap addItemProperties(const QMetaObject* const meta_object) const;
    PropertyMap addParentItemProperties(const QMetaObject* const meta_object) const;

    QString m_name;

    QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem) // GCOVR_EXCL_LINE

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_
