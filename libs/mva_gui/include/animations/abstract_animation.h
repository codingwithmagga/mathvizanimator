/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBS_MVA_GUI_INCLUDE_ANIMATIONS_ABSTRACT_ANIMATION_H_
#define LIBS_MVA_GUI_INCLUDE_ANIMATIONS_ABSTRACT_ANIMATION_H_

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
    enum class State { NOT_STARTED,
        RUNNING,
        DONE };

    explicit AbstractAnimation(QObject* parent = Q_NULLPTR);

    qreal startTime() const { return m_start_time; }
    qreal duration() const { return m_duration; }

    void setStartTime(const qreal start_time);
    void setDuration(const qreal duration);

    State state(const qreal time) const;

    void setFrameTime(const qreal frame_time);

    void setProperties(const QVariantMap& properties);

    virtual QJsonObject toJson() const;
    virtual void applyAnimation(AbstractItem* item, const qreal time) const = 0;

    inline bool operator<(const AbstractAnimation& rhs)
    {
        return startTime() < rhs.startTime();
    }

signals:
    void startTimeChanged(const qreal new_start_time);
    void durationChanged(const qreal new_duration);

private:
    bool isStarted(const qreal time) const;
    bool isDone(const qreal time) const;

    qreal m_start_time = 0.0;
    qreal m_duration = 1.0;

    qreal m_frame_time = 1.0 / 24.0;
};

#endif // LIBS_MVA_GUI_INCLUDE_ANIMATIONS_ABSTRACT_ANIMATION_H_
