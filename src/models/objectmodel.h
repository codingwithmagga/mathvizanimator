#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include "abstractitem.h"

class MObjectModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    MObjectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void addMObject(AbstractItem *object);
    void removeMObject(const int index);

private:
    QList<AbstractItem *> m_objects;

    // QAbstractItemModel interface
};

#endif // OBJECTMODEL_H
