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

#include "fadein.h"

class TestFadeIn : public QObject {
  Q_OBJECT
 private slots:
  void toJsonTest();
};

void TestFadeIn::toJsonTest() {
  const qreal duration = 1.4;
  const qreal start_time = 2.1;

  QJsonObject expected_json;
  expected_json["animation.type"] = "FadeIn";
  expected_json["animation.duration"] = duration;
  expected_json["animation.start_time"] = start_time;

  FadeIn fade_in;
  fade_in.setDuration(duration);
  fade_in.setStartTime(start_time);

  const auto fade_in_json = fade_in.toJson();

  QCOMPARE(fade_in_json, expected_json);
}

QTEST_MAIN(TestFadeIn)
#include "tst_fadein.moc"
