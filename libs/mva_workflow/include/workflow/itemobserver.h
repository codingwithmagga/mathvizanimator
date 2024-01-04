#ifndef ITEMOBSERVER_H
#define ITEMOBSERVER_H

#include "abstractanimation.h"

class ItemObserver {
 public:
  // TODO(codingwithmagga): Prevent use of this
  ItemObserver() = default;
  ItemObserver(QQuickItem* item);

  void setTime(const qreal time);

  bool itemVisible(const qreal time) const;

  void addAnimation(const QSharedPointer<AbstractAnimation>& animation);
  void removeAnimation(const QSharedPointer<AbstractAnimation>& animation);

  QQuickItem* item() const { return m_item; }
  AbstractItem* abstractitem() const {
    return qvariant_cast<AbstractItem*>(m_item->property("item"));
  }

 private:
  QQuickItem* m_item;
  QList<QSharedPointer<AbstractAnimation>> m_animations;
};

Q_DECLARE_METATYPE(ItemObserver)

#endif  // ITEMOBSERVER_H
