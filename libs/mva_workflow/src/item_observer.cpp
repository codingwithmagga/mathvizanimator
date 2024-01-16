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

#include "item_observer.h"

ItemObserver::ItemObserver(QQuickItem *const item, QObject *parent)
    : QObject(parent), m_item(item) {
  const auto item_properties = abstractitem()->getItemProperties();
  const auto quick_item_properties = abstractitem()->getParentItemProperties();

  for (const auto &property : item_properties) {
    m_item_start_property_values.insert(property.first, property.second);
  }

  for (const auto &property : quick_item_properties) {
    m_quick_item_start_property_values.insert(property.first, property.second);
  }
}

void ItemObserver::setTimeProgressive(const qreal time) {
  for (const auto &animation : m_animations) {
    animation->applyAnimation(abstractitem(), time);
  }
}

void ItemObserver::applyStartProperties() {
  const auto item_properties = m_item_start_property_values.keys();
  for (const auto &property : item_properties) {
    abstractitem()->setProperty(property.toUtf8(),
                                m_item_start_property_values.value(property));
  }

  const auto quick_item_properties = m_quick_item_start_property_values.keys();
  for (const auto &property : quick_item_properties) {
    m_item->setProperty(property.toUtf8(),
                        m_quick_item_start_property_values.value(property));
  }
}

void ItemObserver::setTime(const qreal time) {
  applyStartProperties();

  for (const auto &animation : m_animations) {
    if (animation->state(time) == AbstractAnimation::State::NOT_STARTED) {
      continue;
    }

    if (animation->state(time) == AbstractAnimation::State::DONE) {
      animation->applyAnimation(abstractitem(),
                                animation->startTime() + animation->duration());
      continue;
    }
    animation->applyAnimation(abstractitem(), time);
  }
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

void ItemObserver::removeAnimation(
    const QSharedPointer<AbstractAnimation> &animation) {
  m_animations.removeOne(animation);
}

void ItemObserver::removeAnimation(const qint32 animation_number) {
  m_animations.remove(animation_number);
}

void ItemObserver::updateItemProperty(const QString &property,
                                      const QVariant &value) {
  if (abstractitem()->editableProperties().abstract_item_properties.contains(
          property)) {
    abstractitem()->setProperty(property.toUtf8(), value);
    m_item_start_property_values.insert(property.toUtf8(), value);

    abstractitem()->update();
    return;
  }
  m_item->setProperty(property.toUtf8(), value);
  m_quick_item_start_property_values.insert(property.toUtf8(), value);
  m_item->update();
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
