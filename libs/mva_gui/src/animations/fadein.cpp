#include "fadein.h"

FadeIn::FadeIn(QObject *parent) : RealPropertyAnimation{"opacity", parent} {
  setStartPropertyValue(0.0);
  setEndPropertyValue(1.0);
}
