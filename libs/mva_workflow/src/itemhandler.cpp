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

#include "itemhandler.h"

#include "abstractitem.h"
#include "logging.h"

ItemHandler::ItemHandler(QObject* parent) : QObject{parent} {
  QStandardItem* headerItemLeft = new QStandardItem(tr("Name"));
  QStandardItem* headerItemRight = new QStandardItem(tr("Type"));

  m_itemmodel.setHorizontalHeaderItem(0, headerItemLeft);
  m_itemmodel.setHorizontalHeaderItem(1, headerItemRight);

  QStandardItem* propertyHeaderItemLeft = new QStandardItem(tr("Name"));
  QStandardItem* propertyHeaderItemRight = new QStandardItem(tr("Value"));

  m_propertymodel.setHorizontalHeaderItem(0, propertyHeaderItemLeft);
  m_propertymodel.setHorizontalHeaderItem(1, propertyHeaderItemRight);

  connect(&m_propertymodel, &QStandardItemModel::dataChanged, this,
          &ItemHandler::propertyDataChanged);
}

QList<QQuickItem*> ItemHandler::items() {
  QList<QQuickItem*> item_list;

  for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
    const auto model_item = m_itemmodel.item(row)->data(ItemRoles::QUICKITEM);
    item_list.append(model_item.value<QQuickItem*>());
  }

  return item_list;
}

void ItemHandler::clear() { m_itemmodel.removeRows(0, m_itemmodel.rowCount()); }

void ItemHandler::addItem(QQuickItem* const quick_item) {
  if (itemAlreadyExists(quick_item)) {
    return;
  }

  const auto item_extract = extractAbstractItem(quick_item);

  if (item_extract.error) {
    return;
  }

  auto itemName = item_extract.item->name();

  if (itemNameAlreadyExists(itemName)) {
    itemName = prepareNewItemName(itemName);
    item_extract.item->setName(itemName);
  }

  auto stdItemName(new ItemModelItem(itemName));
  auto stdItemType(
      new ItemModelItem(item_extract.item->metaObject()->className()));

  stdItemName->setData(QVariant::fromValue(quick_item), ItemRoles::QUICKITEM);

  m_itemmodel.appendRow(QList<QStandardItem*>{stdItemName, stdItemType});
}

void ItemHandler::removeItem(QQuickItem* const quick_item) {
  const auto item_extract = extractAbstractItem(quick_item);

  if (item_extract.error) {
    return;
  }

  const auto remove_item_name = item_extract.item->name();

  auto item_name_list = m_itemmodel.findItems(remove_item_name);
  for (auto& item : item_name_list) {
    m_itemmodel.removeRow(item->row());
  }
}

// TODO(codingwithmagga): Refactor this function, give useful var names
void ItemHandler::appendProperties(const auto o, auto mo_abstract,
                                   const QStringList& allowedProperties) {
  QList<std::pair<QString, QVariant>> propList;

  for (int i = mo_abstract->propertyOffset(); i < mo_abstract->propertyCount();
       ++i) {
    if (allowedProperties.contains(QString(mo_abstract->property(i).name()))) {
      propList.emplace_back(mo_abstract->property(i).name(),
                            mo_abstract->property(i).read(o.item));
    }
  }

  for (auto& property : propList) {
    auto stdItemName(new QStandardItem(property.first));
    auto stdItemValue(new QStandardItem(property.second.toString()));

    m_propertymodel.appendRow(QList<QStandardItem*>{stdItemName, stdItemValue});
  }
}

// TODO(codingwithmagga): Refactor this function, give useful var names
void ItemHandler::setCurrentRow(const int row) {
  m_propertymodel.removeRows(0, m_propertymodel.rowCount());
  m_currentItemRow = row;

  if (row == -1) {
    return;
  }

  const auto quick_item = m_itemmodel.item(row)
                              ->data(ItemHandler::ItemRoles::QUICKITEM)
                              .value<QQuickItem*>();
  const auto o = extractAbstractItem(quick_item);

  if (o.error) {
    return;
  }

  auto mo_abstract = o.item->metaObject();
  auto mo = quick_item->metaObject();

  const auto allowedProperties = o.item->editableProperties();

  do {
    if (QString(mo_abstract->className()) == "QQuickPaintedItem") {
      break;
    }
    appendProperties(o, mo_abstract,
                     allowedProperties.abstract_item_properties);
  } while ((mo_abstract = mo_abstract->superClass()));

  do {
    appendProperties(o, mo, allowedProperties.quick_item_properties);
  } while ((mo = mo->superClass()));  // TODO(codingwithmagga): sort elements?
}

void ItemHandler::scaleItemsX(const qreal ratio) {
  const auto itemList = items();

  for (auto& item : itemList) {
    item->setX(qRound(item->x() * ratio));
  }
}

void ItemHandler::scaleItemsY(const qreal ratio) {
  const auto itemList = items();

  for (auto& item : itemList) {
    item->setY(qRound(item->y() * ratio));
  }
}

void ItemHandler::scaleItemsWidth(const qreal ratio) {
  const auto itemList = items();

  for (auto& item : itemList) {
    item->setWidth(qRound(item->width() * ratio));
  }
}

void ItemHandler::scaleItemsHeight(const qreal ratio) {
  const auto itemList = items();

  for (auto& item : itemList) {
    item->setHeight(qRound(item->height() * ratio));
  }
}

// TODO(codingwithmagga): Refactor this
// TODO(codingwithmagga): Create custom ItemModels and items which contain
// pointers to the data, s.t. this will be done automatically
void ItemHandler::propertyDataChanged(const QModelIndex& topLeft,
                                      const QModelIndex& bottomRight,
                                      const QList<int>& roles) {
  // Assume only two roles (display + edit) and one Item is changed at a time,
  // otherwise return
  if (topLeft != bottomRight || roles.size() != 2) {
    return;
  }

  // TODO(codingwithmagga): Prevent editing other columns
  if (topLeft.column() != 1) {
    qCWarning(itemhandler) << "Dont change values in column "
                           << topLeft.column() << "in property editor.";
    return;
  }

  if (!roles.contains(Qt::DisplayRole)) {
    return;
  }

  // Set new name in item model
  if (m_propertymodel.data(m_propertymodel.index(topLeft.row(), 0))
          .toString() == "name") {
    QMap<int, QVariant> changedValue;
    changedValue.insert(roles[0], m_propertymodel.data(topLeft));
    m_itemmodel.setItemData(m_itemmodel.index(m_currentItemRow, 0),
                            changedValue);
  }

  // Update item
  auto item = m_itemmodel.item(m_currentItemRow, 0);
  auto quick_item = item->data(ItemRoles::QUICKITEM).value<QQuickItem*>();
  auto itemExtract = extractAbstractItem(
      item->data(ItemRoles::QUICKITEM).value<QQuickItem*>());

  if (itemExtract.error) {
    return;
  }

  auto property =
      m_propertymodel.data(m_propertymodel.index(topLeft.row(), 0)).toString();
  if (itemExtract.item->editableProperties().abstract_item_properties.contains(
          property)) {
    itemExtract.item->setProperty(property.toUtf8(),
                                  m_propertymodel.data(topLeft));
    itemExtract.item->update();
    return;
  }
  quick_item->setProperty(property.toUtf8(), m_propertymodel.data(topLeft));
  quick_item->update();
}

bool ItemHandler::itemAlreadyExists(QQuickItem* const quick_item) {
  for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
    const auto model_item = m_itemmodel.item(row)->data(ItemRoles::QUICKITEM);
    if (model_item.value<QQuickItem*>() == quick_item) {
      return true;
    }
  }
  return false;
}

bool ItemHandler::itemNameAlreadyExists(const QString& name) {
  for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
    const auto rowItemName =
        m_itemmodel.item(row)->data(Qt::DisplayRole).toString();
    if (rowItemName.compare(name) == 0) {
      return true;
    }
  }
  return false;
}

ItemHandler::ItemExtract ItemHandler::extractAbstractItem(
    QQuickItem* const quick_item) {
  ItemHandler::ItemExtract item_extract;

  item_extract.error = false;
  item_extract.item =
      qvariant_cast<AbstractItem*>(quick_item->property("item"));

  if (!item_extract.item) {
    qCWarning(itemhandler)
        << "Given QQuickItem has no item property. Task can't be fulfilled";
    item_extract.error = true;
  }

  return item_extract;
}

QString ItemHandler::prepareNewItemName(const QString& old_item_name) {
  int trailing_num = 1;
  QString new_item_name = old_item_name + "_" + QString::number(trailing_num);

  while (itemNameAlreadyExists(new_item_name)) {
    new_item_name = old_item_name + "_" + QString::number(trailing_num);
    ++trailing_num;
  }

  return new_item_name;
}

ItemModelItem::ItemModelItem(const QString& text) : QStandardItem(text) {}

ItemModelItem::~ItemModelItem() {
  auto storedQuickItem = data(ItemHandler::ItemRoles::QUICKITEM);

  if (storedQuickItem.isValid()) {
    auto quickItem = storedQuickItem.value<QQuickItem*>();

    if (quickItem) {
      quickItem->deleteLater();
    }
  }
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex& index) const {
  auto flags = QAbstractItemModel::flags(index);

  if (index.column() == 1) {
    flags |= Qt::ItemIsEditable;
    return flags;
  }
  return QAbstractItemModel::flags(index);
}
