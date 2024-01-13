#ifndef ITEMOBSERVER_H
#define ITEMOBSERVER_H

#include "abstractanimation.h"

class ItemObserver {
 public:
  // TODO(codingwithmagga): Prevent use of this
  ItemObserver() = default;
  ItemObserver(QQuickItem* item);

  void setTimeProgressiv(const qreal time);
  void setTime(const qreal time);

  bool itemVisible(const qreal time) const;

  void addAnimation(const QSharedPointer<AbstractAnimation>& animation);
  void addAnimations(
      const QList<QSharedPointer<AbstractAnimation>>& animations);
  void removeAnimation(const qint32 animation_number);

  QJsonObject toJson() const;

  QQuickItem* item() const { return m_item; }
  AbstractItem* abstractitem() const {
    return qvariant_cast<AbstractItem*>(m_item->property("item"));
  }

  inline QList<QSharedPointer<AbstractAnimation>> animations() {
    return m_animations;
  }

 private:
  void sortAnimations();

  QQuickItem* m_item;
  QList<QSharedPointer<AbstractAnimation>> m_animations;
};

Q_DECLARE_METATYPE(ItemObserver)

#endif  // ITEMOBSERVER_H
