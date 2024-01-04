#include "realpropertyanimation.h"

RealPropertyAnimation::RealPropertyAnimation(const QString &property,
                                             QObject *parent)
    : PropertyAnimation{property, parent} {}

void RealPropertyAnimation::applyAnimation(AbstractItem *item,
                                           const qreal time) const {
  if (!isRunning(time)) {
    return;
  }

  auto property_value = m_start_property_value +
                        (time - startTime()) *
                            (m_end_property_value - m_start_property_value) /
                            duration();

  if (m_start_property_value < m_end_property_value) {
    property_value =
        qBound(m_start_property_value, property_value, m_end_property_value);
  } else {
    property_value =
        qBound(m_end_property_value, property_value, m_start_property_value);
  }

  item->setProperty(property().toUtf8(), property_value);
}

qreal RealPropertyAnimation::startPropertyValue() const {
  return m_start_property_value;
}

void RealPropertyAnimation::setStartPropertyValue(
    qreal new_start_property_value) {
  if (qFuzzyCompare(m_start_property_value, new_start_property_value)) {
    return;
  }

  m_start_property_value = new_start_property_value;
  emit startPropertyValueChanged();
}

qreal RealPropertyAnimation::endPropertyValue() const {
  return m_end_property_value;
}

void RealPropertyAnimation::setEndPropertyValue(qreal new_end_property_value) {
  if (qFuzzyCompare(m_end_property_value, new_end_property_value)) {
    return;
  }

  m_end_property_value = new_end_property_value;
  emit endPropertyValueChanged();
}
