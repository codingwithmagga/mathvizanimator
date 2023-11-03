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

#ifndef LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEMHANDLER_H_
#define LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEMHANDLER_H_

#include <QObject>
#include <QQuickItem>
#include <QStandardItemModel>

#include "abstractitem.h"

class PropertyModel : public QStandardItemModel {
 public:
  Qt::ItemFlags flags(const QModelIndex& index) const;
};

class ItemHandler : public QObject {
  Q_OBJECT

 public:
  enum ItemRoles { QUICKITEM = Qt::UserRole + 1 };

  explicit ItemHandler(QObject* parent = nullptr);

  qsizetype numItems() const { return m_itemmodel.rowCount(); }
  QStandardItemModel* model() { return &m_itemmodel; }
  QStandardItemModel* propertyModel() { return &m_propertymodel; }
  QList<QQuickItem*> items();

 public slots:
  void clear();

  void addItem(QQuickItem* const quick_item);
  void removeItem(QQuickItem* const quick_item);

  void setCurrentRow(const int row);

  void scaleItemsX(const qreal ratio);
  void scaleItemsY(const qreal ratio);

  void scaleItemsWidth(const qreal ratio);
  void scaleItemsHeight(const qreal ratio);

 private slots:
  void propertyDataChanged(const QModelIndex& topLeft,
                           const QModelIndex& bottomRight,
                           const QList<int>& roles = QList<int>());

 private:
  struct ItemExtract {
    AbstractItem* item;
    bool error;
  };

  bool itemAlreadyExists(QQuickItem* const quick_item);
  bool itemNameAlreadyExists(const QString& name);
  ItemExtract extractAbstractItem(QQuickItem* const quick_item);

  QString prepareNewItemName(const QString& old_item_name);

  void appendProperties(const auto o, auto mo_abstract,
                        const QStringList& allowedProperties);

  QStandardItemModel m_itemmodel;
  PropertyModel m_propertymodel;

  int m_currentItemRow = -1;
};

class ItemModelItem : public QStandardItem {
 public:
  explicit ItemModelItem(const QString& text);

  ~ItemModelItem();
};

#endif  // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEMHANDLER_H_
