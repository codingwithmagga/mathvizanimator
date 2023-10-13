#ifndef ITEMHANDLER_H
#define ITEMHANDLER_H

#include <QObject>
#include <QQuickItem>
#include <QStandardItemModel>

#include "abstractitem.h"

class ItemHandler : public QObject {
    Q_OBJECT
public:
    enum ItemRoles { QUICKITEM = Qt::UserRole + 1 };

    explicit ItemHandler(QObject* parent = nullptr);

    qsizetype numItems() const { return m_itemmodel.rowCount(); }
    QStandardItemModel* model() { return &m_itemmodel; }
    QList<QQuickItem*> items();

public slots:
    void clear();
    void addItem(QQuickItem* const quick_item);
    void removeItem(QQuickItem* const quick_item);

private:
    struct ItemExtract {
        AbstractItem* item;
        bool error;
    };

    bool itemAlreadyExists(QQuickItem* const quick_item);
    bool itemNameAlreadyExists(const QString& name);
    ItemExtract extractAbstractItem(QQuickItem* const quick_item);

    QString prepareNewItemName(const QString& old_item_name);

    QStandardItemModel m_itemmodel;
};

class ItemModelItem : public QStandardItem {
public:
    explicit ItemModelItem(const QString& text);
    ~ItemModelItem();
};

#endif // ITEMHANDLER_H
