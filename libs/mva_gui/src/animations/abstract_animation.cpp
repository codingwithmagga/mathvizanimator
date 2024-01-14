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

#include "abstract_animation.h"

AbstractAnimation::AbstractAnimation(QObject* parent) : QObject(parent) {}

AbstractAnimation::State AbstractAnimation::state(const qreal time) const {
  if (!isStarted(time)) {
    return State::NOT_STARTED;
  }

  if (isDone(time)) {
    return State::DONE;
  }

  return State::RUNNING;
}

void AbstractAnimation::setFrameTime(const qreal frame_time) {
  if (qFuzzyCompare(m_frame_time, frame_time)) {
    return;
  }

  m_frame_time = frame_time;
}

void AbstractAnimation::setProperties(const QVariantMap& properties) {
  QVariantMap::const_iterator iter;
  for (iter = properties.constBegin(); iter != properties.constEnd(); ++iter) {
    setProperty(iter.key().toUtf8(), iter.value());
  }
}

QJsonObject AbstractAnimation::toJson() const {
  QJsonObject json;

  json["type"] = metaObject()->className();
  json["start_time"] = m_start_time;
  json["duration"] = m_duration;

  return json;
}

void AbstractAnimation::setStartTime(const qreal start_time) {
  if (qFuzzyCompare(m_start_time, start_time)) {
    return;
  }

  m_start_time = start_time;
  emit startTimeChanged(m_start_time);
}

void AbstractAnimation::setDuration(const qreal duration) {
  if (qFuzzyCompare(m_duration, duration)) {
    return;
  }

  m_duration = duration;
  emit startTimeChanged(m_duration);
}

bool AbstractAnimation::isStarted(const qreal time) const {
  return time >= m_start_time;
}

bool AbstractAnimation::isDone(const qreal time) const {
  return time >= m_start_time + m_duration + m_frame_time;
}
