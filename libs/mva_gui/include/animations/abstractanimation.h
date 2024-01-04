#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QString>

#include "abstractitem.h"

class AbstractAnimation : public QObject {
  Q_OBJECT

  Q_PROPERTY(qreal start_time READ startTime WRITE setStartTime NOTIFY
                 startTimeChanged FINAL)
  Q_PROPERTY(qreal duration READ duration WRITE setDuration NOTIFY
                 durationChanged FINAL)

 public:
  explicit AbstractAnimation(QObject* parent = Q_NULLPTR);

  qreal startTime() const { return m_start_time; }
  qreal duration() const { return m_duration; }

  void setStartTime(const qreal start_time);
  void setDuration(const qreal duration);

  bool isRunning(const qreal time) const;
  bool isStarted(const qreal time) const;
  bool isDone(const qreal time) const;

  virtual void applyAnimation(AbstractItem* item, const qreal time) const = 0;

  friend bool operator<(const AbstractAnimation& l,
                        const AbstractAnimation& r) {
    return l.startTime() < r.startTime();
  }

  void setFrameTime(const qreal frame_time);

 signals:
  void startTimeChanged(const qreal new_start_time);
  void durationChanged(const qreal new_duration);

 private:
  qreal m_start_time = 0.0;
  qreal m_duration = 1.0;

  qreal m_frame_time = 1.0 / 24.0;
};

#endif  // ANIMATION_H
