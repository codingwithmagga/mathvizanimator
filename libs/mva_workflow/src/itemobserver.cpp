#include "itemobserver.h"

ItemObserver::ItemObserver(QQuickItem *item) : m_item(item) {}

void ItemObserver::setTime(const qreal time) {
  for (const auto &animation : m_animations) {
    animation->applyAnimation(abstractitem(), time);
  }
}

// TODO(codingwithmagga): Implement
bool ItemObserver::itemVisible(const qreal time) const {
  Q_UNUSED(time)
  Q_UNIMPLEMENTED();
  return true;
}

void ItemObserver::addAnimation(
    const QSharedPointer<AbstractAnimation> &animation) {
  m_animations.append(animation);
  std::sort(m_animations.begin(), m_animations.end(),
            [](const QSharedPointer<AbstractAnimation> &a,
               const QSharedPointer<AbstractAnimation> &b) { return *a < *b; });
}

void ItemObserver::removeAnimation(
    const QSharedPointer<AbstractAnimation> &animation) {
  m_animations.removeOne(animation);
}
