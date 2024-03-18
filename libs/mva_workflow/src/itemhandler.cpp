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
#include "basic_item.h"
#include "fade_in.h"
#include "fade_out.h"
#include "geometry_item.h"
#include "logging.h"
#include "textitem.h"

ItemHandler::ItemHandler(QObject* parent)
    : QObject { parent }
{
    QStandardItem* headerItemLeft = new QStandardItem(tr("Name"));
    QStandardItem* headerItemRight = new QStandardItem(tr("Type"));

    m_item_model.setHorizontalHeaderItem(0, headerItemLeft);
    m_item_model.setHorizontalHeaderItem(1, headerItemRight);

    QStandardItem* propertyHeaderItemLeft = new QStandardItem(tr("Name"));
    QStandardItem* propertyHeaderItemRight = new QStandardItem(tr("Value"));

    m_property_model.setHorizontalHeaderItem(0, propertyHeaderItemLeft);
    m_property_model.setHorizontalHeaderItem(1, propertyHeaderItemRight);

    QStandardItem* animationHeaderItemLeft = new QStandardItem(tr("Animation"));
    QStandardItem* animationHeaderItemRight = new QStandardItem(tr("time span"));

    m_animation_model.setHorizontalHeaderItem(0, animationHeaderItemLeft);
    m_animation_model.setHorizontalHeaderItem(1, animationHeaderItemRight);

    connect(&m_property_model, &QStandardItemModel::dataChanged, this, &ItemHandler::propertyDataChanged);
    connect(&m_item_selection_model, &QItemSelectionModel::currentRowChanged, this, &ItemHandler::currentItemChanged);

    m_item_selection_model.setModel(&m_item_model);
}

QList<BasicItem*> ItemHandler::basicItems()
{
    QList<BasicItem*> item_list;

    for (qint32 row = 0; row < m_item_model.rowCount(); ++row) {
        const auto model_item = dynamic_cast<ItemModelItem*>(m_item_model.item(row));
        item_list.append(model_item->itemObserver()->item());
    }

    return item_list;
}

QList<QSharedPointer<ItemObserver>> ItemHandler::items()
{
    QList<QSharedPointer<ItemObserver>> list;

    for (int r = 0; r < m_item_model.rowCount(); ++r) {
        auto index = m_item_model.index(r, 0);
        auto item = m_item_model.itemFromIndex(index);

        list.append(dynamic_cast<ItemModelItem*>(item)->itemObserver());
    }
    return list;
}

void ItemHandler::scaleItems(const qreal width_ratio, const qreal height_ratio)
{
    const auto itemList = basicItems();
    const auto avg_ratio = (width_ratio + height_ratio) / 2.0;

    for (auto& item : itemList) {
        item->setX(qRound(item->x() * width_ratio));
        item->setWidth(qRound(item->width() * width_ratio));
        item->setY(qRound(item->y() * height_ratio));
        item->setHeight(qRound(item->height() * height_ratio));

        auto abstract_item = item->abstractItem();
        auto geometry_item = qobject_cast<GeometryItem*>(abstract_item);
        auto text_item = qobject_cast<TextItem*>(abstract_item);
        if (geometry_item) {
            geometry_item->setBorderWidth(qRound(geometry_item->borderWidth() * avg_ratio));
        }
        if (text_item) {
            text_item->setScaleText(text_item->scaleText() * avg_ratio);
        }
    }
}

void ItemHandler::setDeleteEachQuickItem(QModelIndex parent)
{
    for (int r = 0; r < m_item_model.rowCount(parent); ++r) {
        auto index = m_item_model.index(r, 0, parent);
        auto item = m_item_model.itemFromIndex(index);

        dynamic_cast<ItemModelItem*>(item)->setDeleteQuickitem(true);

        if (m_item_model.hasChildren(index)) {
            setDeleteEachQuickItem(index);
        }
    }
}

ItemModelItem* ItemHandler::getItemModelItemByName(const QString& item_name)
{
    const auto item_list = m_item_model.findItems(item_name);

    if (item_list.isEmpty()) {
        return nullptr;
    }

    return dynamic_cast<ItemModelItem*>(item_list.first());
}

QSharedPointer<ItemObserver> ItemHandler::getItemObserverByName(const QString& item_name)
{
    const auto model_item = getItemModelItemByName(item_name);

    if (!model_item) {
        return nullptr;
    }

    return model_item->itemObserver();
}

void ItemHandler::clear()
{
    setDeleteEachQuickItem();
    m_item_model.removeRows(0, m_item_model.rowCount());
}

void ItemHandler::addItem(BasicItem* const basic_item)
{
    if (itemAlreadyExists(basic_item)) {
        return;
    }

    auto item_name = basic_item->abstractItemName();

    if (itemNameAlreadyExists(item_name)) {
        item_name = prepareNewItemName(item_name);
        basic_item->abstractItem()->setName(item_name);
    }

    const auto item_type = basic_item->abstractItem()->metaObject()->className();
    auto stdItemName(new ItemModelItem(item_name));
    auto stdItemType(new ItemModelItem(item_type));

    const auto item_observer = QSharedPointer<ItemObserver>(new ItemObserver(basic_item));
    stdItemName->setItemObserver(item_observer);

    m_item_model.appendRow(QList<QStandardItem*> { stdItemName, stdItemType });
    qCInfo(itemhandler) << "An item with name" << item_name << "of type" << item_type
                        << "was added at:" << basic_item->position();
}

void ItemHandler::removeItem(BasicItem* const basic_item)
{
    const auto remove_item_name = basic_item->abstractItemName();

    auto item_name_list = m_item_model.findItems(remove_item_name);
    for (auto& item : item_name_list) {
        dynamic_cast<ItemModelItem*>(item)->setDeleteQuickitem(true);
        m_item_model.removeRow(item->row());
    }
}

void ItemHandler::setCurrentItem(const QString& itemName)
{
    const auto itemList = m_item_model.findItems(itemName);

    if (itemList.isEmpty()) {
        qCWarning(itemhandler) << "Can't find item with name:" << itemName << "Current item does not change.";
        return;
    }

    const auto currentItemIndex = m_item_model.indexFromItem(itemList[0]);
    m_item_selection_model.setCurrentIndex(currentItemIndex, QItemSelectionModel::Current | QItemSelectionModel::Rows);
}

void ItemHandler::removeCurrentItem()
{
    const auto current_item = m_item_model.itemFromIndex(m_item_selection_model.currentIndex());
    dynamic_cast<ItemModelItem*>(current_item)->setDeleteQuickitem(true);
    m_item_model.removeRow(current_item->row());
}

void ItemHandler::addAnimation(
    const QString& item_name, const QString& animation_type, const qreal start_time, const qreal duration)
{
    auto item = getItemModelItemByName(item_name);
    if (!item) {
        qCWarning(itemhandler) << "Can't find item with name:" << item_name << "Animation can't be added.";
        return;
    }
    const auto item_observer = item->itemObserver();

    QString animation_name;

    if (animation_type == "FadeIn") {
        auto fade_in_animation = QSharedPointer<FadeIn>(new FadeIn);
        fade_in_animation->setStartTime(start_time);
        fade_in_animation->setDuration(duration);
        item_observer->addAnimation(fade_in_animation);

        animation_name = "FadeIn";

    } else if (animation_type == "FadeOut") {
        auto fade_out_animation = QSharedPointer<FadeOut>(new FadeOut);
        fade_out_animation->setStartTime(start_time);
        fade_out_animation->setDuration(duration);
        item_observer->addAnimation(fade_out_animation);

        animation_name = "FadeOut";
    } else {
        qCWarning(itemhandler) << "Can't find item with name:" << item_name << "Animation can't be added.";
        return;
    }

    const auto index_current_item = m_item_model.index(m_item_selection_model.currentIndex().row(), 0);
    if (item == m_item_model.itemFromIndex(index_current_item)) {
        repopulateAnimationModel(item);
    }

    QString animation_time_span = QString::number(start_time) + "-" + QString::number(start_time + duration);

    qCInfo(itemhandler) << "An animation of type" << animation_name << "with time span" << animation_time_span
                        << "was added to item:" << item_name;
}

void ItemHandler::addAnimations(const QString& item_name, const QList<QSharedPointer<AbstractAnimation>> animations)
{
    auto item_observer = getItemObserverByName(item_name);
    if (!item_observer) {
        qCWarning(itemhandler) << "Can't find item with name:" << item_name << "Animations can't be added.";
        return;
    }
    item_observer->addAnimations(animations);
}

void ItemHandler::removeAnimation(const qint32 animation_number)
{
    const auto index_first_item = m_item_model.index(m_item_selection_model.currentIndex().row(), 0);
    auto currentItem = dynamic_cast<ItemModelItem*>(m_item_model.itemFromIndex(index_first_item));

    if (animation_number > currentItem->itemObserver()->animations().size()) {
        qCCritical(itemhandler) << "Animation can't be removed. Animation number" << animation_number
                                << "is bigger than number of animations stored";
    }

    currentItem->itemObserver()->removeAnimation(animation_number);
    repopulateAnimationModel(currentItem);

    qCInfo(itemhandler) << "Remove animation number" << animation_number << "from item"
                        << currentItem->itemObserver()->abstractitem()->name();
}

// TODO(codingwithmagga): Refactor this function, give useful var names
void ItemHandler::appendProperties(const auto obj, auto meta_object, const QStringList& allowedProperties)
{
    QList<std::pair<QString, QVariant>> propList;

    for (auto i = meta_object->propertyOffset(); i < meta_object->propertyCount(); ++i) {
        if (allowedProperties.contains(QString(meta_object->property(i).name()))) {
            propList.emplace_back(meta_object->property(i).name(), meta_object->property(i).read(obj));
        }
    }

    for (auto& property : propList) {
        auto stdItemName(new QStandardItem(property.first));
        auto stdItemValue(new QStandardItem(property.second.toString()));

        m_property_model.appendRow(QList<QStandardItem*> { stdItemName, stdItemValue });
    }
}

// TODO(codingwithmagga): Refactor this function, give useful var names and use
// AbstractItem::getItemProperties()
// Also change param to ItemModelItem*
void ItemHandler::repopulatePropertyModel(const QModelIndex& currentIndex)
{
    m_property_model.removeRows(0, m_property_model.rowCount());

    const auto model_item = dynamic_cast<ItemModelItem*>(m_item_model.itemFromIndex(currentIndex));
    const auto basic_item = model_item->itemObserver()->item();
    const auto abstract_item = basic_item->abstractItem();

    auto mo_abstract = abstract_item->metaObject();
    auto mo = basic_item->metaObject();

    const auto allowedProperties = abstract_item->editableProperties();

    do {
        appendProperties(abstract_item, mo_abstract, allowedProperties.abstract_item_properties);
    } while ((mo_abstract = mo_abstract->superClass()));

    do {
        appendProperties(basic_item, mo, allowedProperties.basic_item_properties);
    } while ((mo = mo->superClass())); // TODO(codingwithmagga): sort elements?
}

void ItemHandler::repopulateAnimationModel(const ItemModelItem* const item)
{
    m_animation_model.removeRows(0, m_animation_model.rowCount());

    const auto animations = item->itemObserver()->animations();

    for (const auto& animation : animations) {
        QString animation_time_span = QString::number(animation->startTime()) + "-"
            + QString::number(animation->startTime() + animation->duration());

        auto std_item_name(new QStandardItem(animation->metaObject()->className()));
        auto std_item_time_span(new QStandardItem(animation_time_span));

        m_animation_model.appendRow(QList<QStandardItem*> { std_item_name, std_item_time_span });
    }
}

void ItemHandler::setTime(const qreal time)
{
    const auto item_observer_list = items();

    for (auto& item_observer : item_observer_list) {
        item_observer->setTime(time);
    }
}

void ItemHandler::changeProperty(const QString& item_name, const QByteArray& property, const QVariant& value)
{
    const auto item_observer = getItemObserverByName(item_name);

    item_observer->updateItemProperty(property, value);
}

// TODO(codingwithmagga): Refactor this
// TODO(codingwithmagga): Create custom ItemModels and items which contain
// pointers to the data, s.t. this will be done automatically
void ItemHandler::propertyDataChanged(
    const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<qint32>& roles)
{
    // Assume only two roles (display + edit) and one Item is changed at a time,
    // otherwise return
    if (topLeft != bottomRight || roles.size() != 2) {
        return;
    }

    // Give a critical warning when this happens. Should normally be avoided by using PropertyModel class.
    if (topLeft.column() != 1) {
        qCCritical(itemhandler) << "Dont change values in column " << topLeft.column() << "in property editor.";
        return;
    }

    if (!roles.contains(Qt::DisplayRole)) {
        return;
    }

    // Set new name in item model
    if (m_property_model.data(m_property_model.index(topLeft.row(), 0)).toString() == "name") {
        QMap<qint32, QVariant> changedValue;
        changedValue.insert(roles[0], m_property_model.data(topLeft));
        m_item_model.setItemData(m_item_selection_model.currentIndex(), changedValue);
    }

    // Update item
    auto item = dynamic_cast<ItemModelItem*>(m_item_model.item(m_item_selection_model.currentIndex().row()));
    const auto item_observer = item->itemObserver();

    auto property = m_property_model.data(m_property_model.index(topLeft.row(), 0)).toString();
    item_observer->updateItemProperty(property, m_property_model.data(topLeft));
}

void ItemHandler::currentItemChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous)

    m_property_model.removeRows(0, m_property_model.rowCount());
    m_animation_model.removeRows(0, m_animation_model.rowCount());

    if (!current.isValid()) {
        return;
    }

    const auto index_first_item = m_item_model.index(current.row(), 0);
    const auto first_item = dynamic_cast<ItemModelItem*>(m_item_model.itemFromIndex(index_first_item));

    repopulatePropertyModel(index_first_item);
    repopulateAnimationModel(first_item);
}

bool ItemHandler::itemAlreadyExists(BasicItem* const basic_item)
{
    for (qint32 row = 0; row < m_item_model.rowCount(); ++row) {
        const auto model_item = dynamic_cast<ItemModelItem*>(m_item_model.item(row));
        if (model_item->itemObserver()->item() == basic_item) {
            return true;
        }
    }
    return false;
}

bool ItemHandler::itemNameAlreadyExists(const QString& name)
{
    for (qint32 row = 0; row < m_item_model.rowCount(); ++row) {
        const auto rowItemName = m_item_model.item(row)->data(Qt::DisplayRole).toString();
        if (rowItemName.compare(name) == 0) {
            return true;
        }
    }
    return false;
}

QString ItemHandler::prepareNewItemName(const QString& old_item_name)
{
    qint32 trailing_num = 1;
    QString new_item_name = old_item_name + "_" + QString::number(trailing_num);

    while (itemNameAlreadyExists(new_item_name)) {
        new_item_name = old_item_name + "_" + QString::number(trailing_num);
        ++trailing_num;
    }

    return new_item_name;
}

ItemModelItem::ItemModelItem(const QString& text)
    : QStandardItem(text)
{
}

ItemModelItem::~ItemModelItem()
{
    if (m_delete_quick_item) {
        if (m_item_observer && m_item_observer->item()) {
            m_item_observer->item()->deleteLater();
        }
    }
}

QSharedPointer<ItemObserver> ItemModelItem::itemObserver() const { return m_item_observer; }

void ItemModelItem::setItemObserver(const QSharedPointer<ItemObserver>& new_item_observer)
{
    m_item_observer = new_item_observer;
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex& index) const
{
    auto flags = QAbstractItemModel::flags(index);

    if (index.column() == 1) {
        flags |= Qt::ItemIsEditable;
        return flags;
    }
    return QAbstractItemModel::flags(index);
}
