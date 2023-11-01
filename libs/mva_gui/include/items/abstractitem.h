/* mathvizanimator
 * Copyright (C) 2023  codingwithmagga
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
#include <QVector>
#include <QtQuick/QQuickPaintedItem>

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

  explicit AbstractItem(const QString& qml_file, QQuickItem* parent = nullptr);

  QString name() const;
  void setName(const QString& name);

  QColor color() const;
  void setColor(const QColor& color);

  virtual QJsonObject toJson() const;
  virtual void setPropertiesFromJson(const QJsonObject& json);

  virtual EditableProperties editableProperties() const;

 signals:
  void nameChanged(const QString& new_name);
  void colorChanged(const QColor& new_color);

 private:
  QString m_name;
  QColor m_color;

  QString m_qml_file;
};

Q_DECLARE_METATYPE(AbstractItem)  // GCOVR_EXCL_LINE

#endif  // LIBS_MVA_GUI_INCLUDE_ITEMS_ABSTRACTITEM_H_
