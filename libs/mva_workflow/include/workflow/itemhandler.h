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

#include <QItemSelectionModel>
#include <QObject>
#include <QQuickItem>
#include <QStandardItemModel>

#include "item_observer.h"

class ItemModelItem;

class PropertyModel : public QStandardItemModel {
  public:
    Qt::ItemFlags flags(const QModelIndex& index) const;
};

class ItemHandler : public QObject {
    Q_OBJECT

  public:
    enum ItemRoles { ITEM_OBSERVER = Qt::UserRole + 1 };

    explicit ItemHandler(QObject* parent = nullptr);

    qsizetype numItems() const { return m_item_model.rowCount(); }
    QStandardItemModel* model() { return &m_item_model; }
    QItemSelectionModel* selectionModel() { return &m_item_selection_model; }
    QStandardItemModel* propertyModel() { return &m_property_model; }
    QStandardItemModel* animationModel() { return &m_animation_model; }
    QList<QQuickItem*> quickItems();
    QList<QSharedPointer<ItemObserver>> items();

  public slots:
    void clear();

    void addItem(QQuickItem* const quick_item,
        const QList<QSharedPointer<AbstractAnimation>>& animations = QList<QSharedPointer<AbstractAnimation>> {});
    void removeItem(QQuickItem* const quick_item);
    void setCurrentItem(const QString& itemName);
    void removeCurrentItem();

    void addAnimation(
        const QString& item_name, const QString& animation_type, const qreal start_time, const qreal duration);
    void removeAnimation(const qint32 animation_number);

    void scaleItemsX(const qreal ratio);
    void scaleItemsY(const qreal ratio);

    void scaleItemsWidth(const qreal ratio);
    void scaleItemsHeight(const qreal ratio);

    void setTime(const qreal time);

  private slots:
    void propertyDataChanged(
        const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<qint32>& roles = QList<qint32>());
    void currentItemChanged(const QModelIndex& current, const QModelIndex& previous);

  private:
    struct ItemExtract {
        AbstractItem* item;
        bool error;
    };

    bool itemAlreadyExists(QQuickItem* const quick_item);
    bool itemNameAlreadyExists(const QString& name);
    ItemExtract extractAbstractItem(QQuickItem* const quick_item);

    QString prepareNewItemName(const QString& old_item_name);

    void appendProperties(const auto obj, auto meta_object, const QStringList& allowedProperties);

    void repopulatePropertyModel(const QModelIndex& currentIndex);
    void repopulateAnimationModel(const ItemModelItem* const item);

    void setDeleteEachQuickItem(QModelIndex parent = QModelIndex());

    QStandardItemModel m_item_model;
    QStandardItemModel m_animation_model;
    QItemSelectionModel m_item_selection_model;

    PropertyModel m_property_model;
};

class ItemModelItem : public QStandardItem {
  public:
    explicit ItemModelItem(const QString& text);

    ~ItemModelItem();

    void setDeleteQuickitem(const bool delete_quick_item) { m_delete_quick_item = delete_quick_item; }

    QSharedPointer<ItemObserver> itemObserver() const;
    void setItemObserver(const QSharedPointer<ItemObserver>& new_item_observer);

  private:
    /*
     * This variable is necessary to determine if the (possibly) containing
     * QQuickItem should be deleted or not. If the user deletes the item it should
     * be set to true, to remove the item from the GUI. If the app gets closed,
     * everything gets automatically deleted from the GUI and this variable should
     * be set (or keeped) false. This is because in some cases both the GUI and
     * the deconstructor of this class seems to try to delete the item
     * simultaneously. This leads to a segmentation fault.
     *
     * I'm not really happy with this solution, because current extensions could
     * break this workflow. This should be done automatically.
     *
     * TODO(codingwithmagga): GUI deletes the QQuickItem. The QQuickItem or
     * better the containing AbstractItem should emit a signal or sth. similar
     * when they get destroyed. Then this ItemModelItem should removed. So it
     * goes the other way around. Not sure if this solves the problem when
     * shutting down the app.
     */
    bool m_delete_quick_item = false;

    QSharedPointer<ItemObserver> m_item_observer;
};

#endif // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEMHANDLER_H_
