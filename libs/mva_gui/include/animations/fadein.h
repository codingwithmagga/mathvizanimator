#ifndef FADEIN_H
#define FADEIN_H

#include "realpropertyanimation.h"

class FadeIn : public RealPropertyAnimation {
 public:
  explicit FadeIn(QObject *parent = nullptr);

 private:
  void setStartPropertyValue(qreal new_start_property_value) {
    RealPropertyAnimation::setStartPropertyValue(new_start_property_value);
  }
  void setEndPropertyValue(qreal new_end_property_value) {
    RealPropertyAnimation::setEndPropertyValue(new_end_property_value);
  }
};

#endif  // FADEIN_H
