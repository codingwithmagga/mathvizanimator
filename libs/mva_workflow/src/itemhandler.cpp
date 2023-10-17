#include "itemhandler.h"

#include "abstractitem.h"
#include "logging.h"

ItemHandler::ItemHandler(QObject *parent)
    : QObject{parent}
{
    QStandardItem *headerItemLeft = new QStandardItem(tr("Name"));
    QStandardItem *headerItemRight = new QStandardItem(tr("Type"));

    m_itemmodel.setHorizontalHeaderItem(0, headerItemLeft);
    m_itemmodel.setHorizontalHeaderItem(1, headerItemRight);

    QStandardItem *propertyHeaderItemLeft = new QStandardItem(tr("Name"));
    QStandardItem *propertyHeaderItemRight = new QStandardItem(tr("Value"));

    m_propertymodel.setHorizontalHeaderItem(0, propertyHeaderItemLeft);
    m_propertymodel.setHorizontalHeaderItem(1, propertyHeaderItemRight);
}

QList<QQuickItem *> ItemHandler::items()
{
    QList<QQuickItem *> item_list;

    for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
        const auto model_item = m_itemmodel.item(row)->data(ItemRoles::QUICKITEM);
        item_list.append(model_item.value<QQuickItem *>());
    }

    return item_list;
}

void ItemHandler::clear()
{
    m_itemmodel.removeRows(0, m_itemmodel.rowCount());
}

void ItemHandler::addItem(QQuickItem *const quick_item)
{
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
    auto stdItemType(new ItemModelItem(item_extract.item->metaObject()->className()));

    stdItemName->setData(QVariant::fromValue(quick_item), ItemRoles::QUICKITEM);

    m_itemmodel.appendRow(QList<QStandardItem *>{stdItemName, stdItemType});
}

void ItemHandler::removeItem(QQuickItem *const quick_item)
{
    const auto item_extract = extractAbstractItem(quick_item);

    if (item_extract.error) {
        return;
    }

    const auto remove_item_name = item_extract.item->name();

    auto item_name_list = m_itemmodel.findItems(remove_item_name);
    for (auto &item : item_name_list) {
        m_itemmodel.removeRow(item->row());
    }
}

void ItemHandler::setCurrentRow(const int row)
{
    m_propertymodel.removeRows(0, m_propertymodel.rowCount());

    if (row == -1) {
        return;
    }

    const auto quick_item
        = m_itemmodel.item(row)->data(ItemHandler::ItemRoles::QUICKITEM).value<QQuickItem *>();
    const auto o = extractAbstractItem(quick_item).item;

    auto mo = o->metaObject();
    qDebug() << "## Properties of" << o << "##";
    do {
        if (QString(mo->className()).compare("QQuickPaintedItem") == 0) {
            break;
        }
        qDebug() << "### Class" << mo->className() << "###";
        std::vector<std::pair<QString, QVariant> > v;
        v.reserve(mo->propertyCount() - mo->propertyOffset());
        for (int i = mo->propertyOffset(); i < mo->propertyCount(); ++i) {
            if (QString(mo->property(i).name()) == "file") {
                continue;
            }
            v.emplace_back(mo->property(i).name(), mo->property(i).read(o));
        }
        for (auto &i : v) {
            qDebug() << i.first << "=>" << i.second;

            auto stdItemName(new QStandardItem(i.first));
            auto stdItemValue(new QStandardItem(i.second.toString()));

            m_propertymodel.appendRow(QList<QStandardItem *>{stdItemName, stdItemValue});
        }
    } while ((mo = mo->superClass()));
}

bool ItemHandler::itemAlreadyExists(QQuickItem *const quick_item)
{
    for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
        const auto model_item = m_itemmodel.item(row)->data(ItemRoles::QUICKITEM);
        if (model_item.value<QQuickItem *>() == quick_item) {
            return true;
        }
    }
    return false;
}

bool ItemHandler::itemNameAlreadyExists(const QString &name)
{
    for (int row = 0; row < m_itemmodel.rowCount(); ++row) {
        const auto rowItemName = m_itemmodel.item(row)->data(Qt::DisplayRole).toString();
        if (rowItemName.compare(name) == 0) {
            return true;
        }
    }
    return false;
}

ItemHandler::ItemExtract ItemHandler::extractAbstractItem(QQuickItem *const quick_item)
{
    ItemHandler::ItemExtract item_extract;

    item_extract.error = false;
    item_extract.item = qvariant_cast<AbstractItem *>(quick_item->property("item"));

    if (!item_extract.item) {
        qCWarning(itemhandler) << "Given QQuickItem has no item property. Task can't be fullfilled";
        item_extract.error = true;
    }

    return item_extract;
}

QString ItemHandler::prepareNewItemName(const QString &old_item_name)
{
    int trailing_num = 1;
    QString new_item_name = old_item_name + "_" + QString::number(trailing_num);

    while (itemNameAlreadyExists(new_item_name)) {
        new_item_name = old_item_name + "_" + QString::number(trailing_num);
        ++trailing_num;
    }

    return new_item_name;
}

ItemModelItem::ItemModelItem(const QString &text)
    : QStandardItem(text)
{}

ItemModelItem::~ItemModelItem()
{
    auto storedQuickItem = data(ItemHandler::ItemRoles::QUICKITEM);

    if (storedQuickItem.isValid()) {
        auto quickItem = storedQuickItem.value<QQuickItem *>();

        if (quickItem) {
            quickItem->deleteLater();
        }
    }
}
