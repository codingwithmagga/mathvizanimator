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

#include <QTest>

#include "circleitem.h"
#include "real_property_animation.h"

class InvalidAnimation : public RealPropertyAnimation {
  public:
    explicit InvalidAnimation(const QString& property, QObject* parent = Q_NULLPTR)
        : RealPropertyAnimation(property, parent)
    {
        setStartPropertyValues({ 1.0, 2.0 });
        setEndPropertyValues({ 1.0 });
    };
};

class TestPropertyAnimation : public QObject {
    Q_OBJECT
  private slots:
    void applyInvalidAnimation();

  private:
};

void TestPropertyAnimation::applyInvalidAnimation()
{
    CircleItem circle_item;
    InvalidAnimation invalid_animation("opacity");
    const qreal test_opacity = 0.5;
    circle_item.setOpacity(test_opacity);
    invalid_animation.applyAnimation(&circle_item, 0.4);

    QCOMPARE(circle_item.opacity(), test_opacity);
}

QTEST_MAIN(TestPropertyAnimation)
#include "tst_property_animation.moc"
