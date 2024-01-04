#include "propertyanimation.h"

PropertyAnimation::PropertyAnimation(const QString &property, QObject *parent)
    : AbstractAnimation{parent}, m_property(property) {}
