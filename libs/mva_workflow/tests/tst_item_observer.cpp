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
#include "fade_in.h"
#include "fade_out.h"
#include "item_observer.h"

class TestItemObserver : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void setTimeTest_data();
  void setTimeTest();

  void timeTestFadeOut_data();
  void timeTestFadeOut();

  void setTimeProgressiveTest();

  void jsonConversionTest();
  void animationsSortedTest();

 private:
  void applyAndCheckTime(const qreal time, const qreal opacity,
                         const QList<qreal> &initial_time_list);

  QSharedPointer<ItemObserver> m_item_observer;
  QSharedPointer<ItemObserver> m_item_observer_no_animations;
  QSharedPointer<FadeIn> m_fade_in_animation;
  QSharedPointer<FadeOut> m_fade_out_animation;
  QSharedPointer<QQuickItem> m_parent_item;
};

void TestItemObserver::initTestCase() {
  const qreal width = 244;
  const qreal height = width;

  m_parent_item = QSharedPointer<QQuickItem>(new QQuickItem());
  m_parent_item->setX(23);
  m_parent_item->setY(155);
  m_parent_item->setHeight(width);
  m_parent_item->setWidth(height);

  auto circle = new CircleItem();
  circle->setWidth(width);
  circle->setHeight(height);
  circle->setColor("blue");
  circle->setOpacity(0.0);
  circle->setParentItem(m_parent_item.data());
  m_parent_item->setProperty("item", QVariant::fromValue<CircleItem *>(circle));

  m_item_observer =
      QSharedPointer<ItemObserver>(new ItemObserver(m_parent_item.data()));
  m_item_observer_no_animations =
      QSharedPointer<ItemObserver>(new ItemObserver(m_parent_item.data()));

  m_fade_in_animation = QSharedPointer<FadeIn>(new FadeIn());
  m_fade_in_animation->setStartTime(0.0);
  m_fade_in_animation->setDuration(1.0);
  m_fade_out_animation = QSharedPointer<FadeOut>(new FadeOut());
  m_fade_out_animation->setStartTime(2.2);
  m_fade_out_animation->setDuration(1.6);

  m_item_observer->addAnimation(m_fade_in_animation);
  m_item_observer->addAnimation(m_fade_out_animation);
}

void TestItemObserver::setTimeTest_data() {
  QTest::addColumn<qreal>("time");
  QTest::addColumn<qreal>("opacity");

  /* jscpd:ignore-start */
  QTest::newRow("fade_in_start") << 0.0 << 0.0;
  QTest::newRow("fade_in_40_percent") << 0.4 << 0.4;
  QTest::newRow("fade_in_end") << 1.0 << 1.0;
  QTest::newRow("between_animations_1") << 1.3 << 1.0;
  QTest::newRow("between_animations_2") << 1.8 << 1.0;
  QTest::newRow("fade_out_start") << 2.2 << 1.0;
  QTest::newRow("fade_out_25_percent") << 2.6 << 0.75;
  QTest::newRow("fade_out_75_percent") << 3.4 << 0.25;
  QTest::newRow("fade_out_end") << 3.8 << 0.0;
  QTest::newRow("end_project") << 5.0 << 0.0;
  /* jscpd:ignore-end */
}

void TestItemObserver::setTimeTest() {
  QFETCH(qreal, time);
  QFETCH(qreal, opacity);

  m_item_observer->updateItemProperty("opacity", 0.0);

  const QList<qreal> initial_time_list = {0.0, 0.5, 1.2, 1.7,
                                          2.4, 2.9, 3.5, 4.0};
  applyAndCheckTime(time, opacity, initial_time_list);
}

void TestItemObserver::timeTestFadeOut_data() {
  QTest::addColumn<qreal>("time");
  QTest::addColumn<qreal>("opacity");

  /* jscpd:ignore-start */
  QTest::newRow("start_project") << 0.0 << 1.0;
  QTest::newRow("fade_out_start") << 2.2 << 1.0;
  QTest::newRow("fade_out_25_percent") << 2.6 << 0.75;
  QTest::newRow("fade_out_75_percent") << 3.4 << 0.25;
  QTest::newRow("fade_out_end") << 3.8 << 0.0;
  QTest::newRow("end_project") << 5.0 << 0.0;
  /* jscpd:ignore-end */
}

void TestItemObserver::timeTestFadeOut() {
  QFETCH(qreal, time);
  QFETCH(qreal, opacity);

  m_item_observer->removeAnimation(m_fade_in_animation);
  m_item_observer->updateItemProperty("opacity", 1.0);

  const QList<qreal> initial_time_list = {0.0, 0.5, 1.2, 1.7,
                                          2.4, 2.9, 3.5, 4.0};
  applyAndCheckTime(time, opacity, initial_time_list);

  m_item_observer->addAnimation(m_fade_in_animation);
}

void TestItemObserver::setTimeProgressiveTest() {
  const QList<qreal> time{0.0, 0.4, 1.0, 1.3, 1.8, 2.2, 2.6, 3.4, 3.8, 5.0};
  const QList<qreal> opacity{0.0, 0.4,  1.0,  1.0, 1.0,
                             1.0, 0.75, 0.25, 0.0, 0.0};

  for (int i = 0; i < time.size(); ++i) {
    m_item_observer->setTime(time[i]);
    QCOMPARE(m_item_observer->abstractitem()->opacity(), opacity[i]);
  }
}

void TestItemObserver::jsonConversionTest() {
  QJsonObject expected_json = m_item_observer->abstractitem()->toJson();
  QJsonObject animation_json_0 = m_fade_in_animation->toJson();
  QJsonObject animation_json_1 = m_fade_out_animation->toJson();

  expected_json["animation_0"] = animation_json_0;
  expected_json["animation_1"] = animation_json_1;

  QCOMPARE(expected_json, m_item_observer->toJson());
}

void TestItemObserver::animationsSortedTest() {
  auto fade_in_1 = QSharedPointer<FadeIn>(new FadeIn);
  auto fade_in_2 = QSharedPointer<FadeIn>(new FadeIn);
  auto fade_in_3 = QSharedPointer<FadeIn>(new FadeIn);
  auto fade_in_4 = QSharedPointer<FadeIn>(new FadeIn);
  auto fade_out_1 = QSharedPointer<FadeOut>(new FadeOut);
  auto fade_out_2 = QSharedPointer<FadeOut>(new FadeOut);
  auto fade_out_3 = QSharedPointer<FadeOut>(new FadeOut);

  fade_in_1->setStartTime(0.6);
  fade_in_2->setStartTime(1.6);
  fade_in_3->setStartTime(2.6);
  fade_in_4->setStartTime(1.2);
  fade_out_1->setStartTime(2.2);
  fade_out_2->setStartTime(3.2);
  fade_out_3->setStartTime(0.2);

  m_item_observer_no_animations->addAnimations({fade_in_1, fade_in_2, fade_in_3,
                                                fade_in_4, fade_out_1,
                                                fade_out_2, fade_out_3});

  QList<QSharedPointer<AbstractAnimation>> expected_order{
      fade_out_3, fade_in_1, fade_in_4, fade_in_2,
      fade_out_1, fade_in_3, fade_out_2};
  QCOMPARE(m_item_observer_no_animations->animations(), expected_order);
}

void TestItemObserver::applyAndCheckTime(
    const qreal time, const qreal opacity,
    const QList<qreal> &initial_time_list) {
  for (const auto initial_time : initial_time_list) {
    m_item_observer->setTime(initial_time);
    m_item_observer->setTime(time);

    QCOMPARE(m_item_observer->abstractitem()->opacity(), opacity);
  }
}

QTEST_MAIN(TestItemObserver)
#include "tst_item_observer.moc"
