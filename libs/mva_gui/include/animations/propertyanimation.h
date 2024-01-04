#ifndef PROPERTYANIMATION_H
#define PROPERTYANIMATION_H

#include <abstractanimation.h>

class PropertyAnimation : public AbstractAnimation {
 public:
  explicit PropertyAnimation(const QString& property,
                             QObject* parent = nullptr);

  QString property() const { return m_property; };

 private:
  const QString m_property;
};

#endif  // PROPERTYANIMATION_H
