#include "fadeout.h"

FadeOut::FadeOut(QObject *parent) : RealPropertyAnimation{"opacity", parent} {
  setStartPropertyValue(1.0);
  setEndPropertyValue(0.0);
}
