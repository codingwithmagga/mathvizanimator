#ifndef REALPROPERTYANIMATION_H
#define REALPROPERTYANIMATION_H

#include "propertyanimation.h"

class RealPropertyAnimation : public PropertyAnimation {
  Q_OBJECT

  Q_PROPERTY(qreal start_property_value READ startPropertyValue WRITE
                 setStartPropertyValue NOTIFY startPropertyValueChanged)
  Q_PROPERTY(qreal end_property_value READ endPropertyValue WRITE
                 setEndPropertyValue NOTIFY endPropertyValueChanged)

 public:
  explicit RealPropertyAnimation(const QString &property,
                                 QObject *parent = Q_NULLPTR);

  void applyAnimation(AbstractItem *item, const qreal time) const override;

  qreal startPropertyValue() const;
  void setStartPropertyValue(qreal new_start_property_value);

  qreal endPropertyValue() const;
  void setEndPropertyValue(qreal new_end_property_value);

 signals:
  void startPropertyValueChanged();
  void endPropertyValueChanged();

 private:
  qreal m_start_property_value = 0.0;
  qreal m_end_property_value = 1.0;
};

#endif  // REALPROPERTYANIMATION_H
