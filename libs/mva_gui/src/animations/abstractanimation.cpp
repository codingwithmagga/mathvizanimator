#include "abstractanimation.h"

AbstractAnimation::AbstractAnimation(QObject* parent) : QObject(parent) {}

bool AbstractAnimation::isRunning(const qreal time) const {
  return (isStarted(time) && !isDone(time));
}

bool AbstractAnimation::isStarted(const qreal time) const {
  return time >= m_start_time;
}

bool AbstractAnimation::isDone(const qreal time) const {
  return time >= m_start_time + m_duration + m_frame_time;
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
