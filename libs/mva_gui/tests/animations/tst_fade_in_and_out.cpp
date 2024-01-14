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

class TestFadeInAndOut : public QObject {
  Q_OBJECT
 private slots:
  void initTestCase();

  void stateTest_data();
  void stateTest();

  void toJsonTest();
  void defaultPropertyValueTest();

  void propertyValueTest_data();
  void propertyValueTest();

  void applyAnimationTest_data();
  void applyAnimationTest();

 private:
  FadeIn m_fade_in;
  FadeOut m_fade_out;
  CircleItem m_circle_item_fade_in;
  CircleItem m_circle_item_fade_out;

  const qreal m_duration_fade_in = 1.4;
  const qreal m_start_time_fade_in = 2.1;
  const qreal m_duration_fade_out = 0.8;
  const qreal m_start_time_fade_out = 0.5;
  const qreal m_frame_time = 1.0 / 24.0;
};

void TestFadeInAndOut::initTestCase() {
  m_fade_in.setDuration(m_duration_fade_in);
  m_fade_in.setStartTime(m_start_time_fade_in);
  m_fade_in.setFrameTime(m_frame_time);

  m_fade_out.setDuration(m_duration_fade_out);
  m_fade_out.setStartTime(m_start_time_fade_out);
  m_fade_out.setFrameTime(m_frame_time);

  m_circle_item_fade_in.setOpacity(0.0);
  m_circle_item_fade_out.setOpacity(1.0);
}

void TestFadeInAndOut::stateTest_data() {
  QTest::addColumn<qreal>("time");
  QTest::addColumn<AbstractAnimation::State>("state");

  QTest::newRow("start_project")
      << 0.0 << AbstractAnimation::State::NOT_STARTED;
  QTest::newRow("before_animation_start")
      << 1.5 << AbstractAnimation::State::NOT_STARTED;
  QTest::newRow("very_close_before_animation_start")
      << 2.099 << AbstractAnimation::State::NOT_STARTED;
  QTest::newRow("animation_start") << 2.1 << AbstractAnimation::State::RUNNING;
  QTest::newRow("middle_of_animation")
      << 3.0 << AbstractAnimation::State::RUNNING;
  QTest::newRow("end_of_animation") << 3.5 << AbstractAnimation::State::RUNNING;
  QTest::newRow("end_of_animation_plus_safety_value")
      << 3.5 + m_frame_time - 0.01 << AbstractAnimation::State::RUNNING;
  QTest::newRow("after_animation") << 3.6 << AbstractAnimation::State::DONE;
  QTest::newRow("project_end") << 5.0 << AbstractAnimation::State::DONE;
}

void TestFadeInAndOut::stateTest() {
  QFETCH(qreal, time);
  QFETCH(AbstractAnimation::State, state);

  QCOMPARE(m_fade_in.state(time), state);
}

void TestFadeInAndOut::toJsonTest() {
  QJsonObject expected_json;
  expected_json["type"] = "FadeIn";
  expected_json["duration"] = m_duration_fade_in;
  expected_json["start_time"] = m_start_time_fade_in;

  const auto fade_in_json = m_fade_in.toJson();

  QCOMPARE(fade_in_json, expected_json);
}

void TestFadeInAndOut::defaultPropertyValueTest() {
  QCOMPARE(m_fade_in.duration(), m_duration_fade_in);
  QCOMPARE(m_fade_in.startTime(), m_start_time_fade_in);

  QCOMPARE(m_fade_in.startPropertyValue(), 0.0);
  QCOMPARE(m_fade_in.endPropertyValue(), 1.0);
  QCOMPARE(m_fade_in.property(), "opacity");

  QCOMPARE(m_fade_out.startPropertyValue(), 1.0);
  QCOMPARE(m_fade_out.endPropertyValue(), 0.0);
  QCOMPARE(m_fade_out.property(), "opacity");
}

void TestFadeInAndOut::propertyValueTest_data() {
  QTest::addColumn<QVariantMap>("properties");

  QTest::newRow("default_values")
      << QVariantMap{std::pair<QString, QVariant>("start_time", QVariant(0)),
                     std::pair<QString, QVariant>("duration", QVariant(1))};
  QTest::newRow("simple_values_1")
      << QVariantMap{std::pair<QString, QVariant>("start_time", QVariant(0.6)),
                     std::pair<QString, QVariant>("duration", QVariant(1.6))};
  QTest::newRow("simple_values_2")
      << QVariantMap{std::pair<QString, QVariant>("start_time", QVariant(3.2)),
                     std::pair<QString, QVariant>("duration", QVariant(0.5))};
}

void TestFadeInAndOut::propertyValueTest() {
  QFETCH(QVariantMap, properties);

  FadeIn fade_in;
  FadeIn fade_out;
  fade_in.setProperties(properties);
  fade_out.setProperties(properties);

  QCOMPARE(fade_in.startTime(), properties["start_time"]);
  QCOMPARE(fade_in.duration(), properties["duration"]);
  QCOMPARE(fade_out.startTime(), properties["start_time"]);
  QCOMPARE(fade_out.duration(), properties["duration"]);
}

void TestFadeInAndOut::applyAnimationTest_data() {
  QTest::addColumn<qreal>("time");
  QTest::addColumn<qreal>("opacity_fade_in");
  QTest::addColumn<qreal>("opacity_fade_out");

  QTest::newRow("start_project") << 0.0 << 0.0 << 1.0;
  QTest::newRow("before_fade_out_animation_start") << 0.25 << 0.0 << 1.0;
  QTest::newRow("very_close_before_fade_out_animation_start")
      << 0.499 << 0.0 << 1.0;
  QTest::newRow("fade_out_animation_start") << 0.5 << 0.0 << 1.0;
  QTest::newRow("middle_of_fade_out_animation") << 0.9 << 0.0 << 0.5;
  QTest::newRow("end_of_fade_out_animation") << 1.3 << 0.0 << 0.0;
  QTest::newRow("end_of_fade_out_animation_plus_safety_value")
      << 1.3 + m_frame_time - 0.01 << 0.0 << 0.0;
  QTest::newRow("after_fade_out_animation") << 1.4 << 0.0 << 0.0;
  QTest::newRow("before_fade_in_animation_start") << 1.5 << 0.0 << 0.0;
  QTest::newRow("very_close_before_fade_in_animation_start")
      << 2.099 << 0.0 << 0.0;
  QTest::newRow("fade_in_animation_start") << 2.1 << 0.0 << 0.0;
  QTest::newRow("middle_of_fade_in_animation") << 2.8 << 0.5 << 0.0;
  QTest::newRow("end_of_fade_in_animation") << 3.5 << 1.0 << 0.0;
  QTest::newRow("end_of_fade_in_animation_plus_safety_value")
      << 3.5 + m_frame_time - 0.01 << 1.0 << 0.0;
  QTest::newRow("after_fade_in_animation") << 3.6 << 1.0 << 0.0;
  QTest::newRow("project_end") << 5.0 << 1.0 << 0.0;
}

void TestFadeInAndOut::applyAnimationTest() {
  QFETCH(qreal, time);
  QFETCH(qreal, opacity_fade_in);
  QFETCH(qreal, opacity_fade_out);

  m_fade_in.applyAnimation(&m_circle_item_fade_in, time);
  m_fade_out.applyAnimation(&m_circle_item_fade_out, time);

  QCOMPARE(m_circle_item_fade_in.opacity(), opacity_fade_in);
  QCOMPARE(m_circle_item_fade_out.opacity(), opacity_fade_out);
}

QTEST_MAIN(TestFadeInAndOut)
#include "tst_fade_in_and_out.moc"
