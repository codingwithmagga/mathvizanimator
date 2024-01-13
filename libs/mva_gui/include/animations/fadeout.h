#ifndef FADEOUT_H
#define FADEOUT_H

#include "realpropertyanimation.h"

class FadeOut : public RealPropertyAnimation {
  Q_OBJECT

 public:
  explicit FadeOut(QObject *parent = nullptr);

 private:
  void setStartPropertyValue(qreal new_start_property_value) {
    RealPropertyAnimation::setStartPropertyValue(new_start_property_value);
  }
  void setEndPropertyValue(qreal new_end_property_value) {
    RealPropertyAnimation::setEndPropertyValue(new_end_property_value);
  }
};

#endif  // FADEOUT_H
