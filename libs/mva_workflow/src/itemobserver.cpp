#include "itemobserver.h"

ItemObserver::ItemObserver(QQuickItem *item) : m_item(item) {}

void ItemObserver::setTimeProgressiv(const qreal time) {
  for (const auto &animation : m_animations) {
    animation->applyAnimation(abstractitem(), time);
  }
}

void ItemObserver::setTime(const qreal time) {
  for (const auto &animation : m_animations) {
    if (!animation->isStarted(time)) {
      continue;
    }

    if (animation->isDone(time)) {
      animation->applyAnimation(abstractitem(),
                                animation->startTime() + animation->duration());
      continue;
    }
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
  sortAnimations();
}

void ItemObserver::addAnimations(
    const QList<QSharedPointer<AbstractAnimation> > &animations) {
  m_animations.append(animations);
  sortAnimations();
}

void ItemObserver::removeAnimation(const qint32 animation_number) {
  m_animations.remove(animation_number);
}

QJsonObject ItemObserver::toJson() const {
  auto json_item = abstractitem()->toJson();

  qint32 animation_count = 0;
  for (const auto &animation : m_animations) {
    json_item["animation_" + QString::number(animation_count)] =
        animation->toJson();
    animation_count++;
  }

  return json_item;
}

void ItemObserver::sortAnimations() {
  std::sort(m_animations.begin(), m_animations.end(),
            [](const QSharedPointer<AbstractAnimation> &a,
               const QSharedPointer<AbstractAnimation> &b) { return *a < *b; });
}
