#include "objectmodel.h"

MObjectModel::MObjectModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void MObjectModel::addMObject(AbstractItem *object)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_objects.append(object);
    endInsertRows();
}

void MObjectModel::removeMObject(const int index)
{
    m_objects.remove(index);
}

int MObjectModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_objects.count();
}

QVariant MObjectModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_objects.count())
        return QVariant();

    auto object = m_objects[index.row()];
    if (role == Qt::DisplayRole) {
        return object->name() + " " + QString::number(object->parentItem()->x()) + " "
               + QString::number(object->parentItem()->y());
    }

    return QVariant();
}
