#ifndef OBJECTMODEL_H
#define OBJECTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include "abstractobject.h"

class MObjectModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    MObjectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
    void addMObject(AbstractObject *object);

private:
    QList<AbstractObject *> m_objects;
};

#endif // OBJECTMODEL_H
