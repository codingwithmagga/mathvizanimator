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
#include "fadein.h"
#include "fadeout.h"
#include "itemobserver.h"

class TestItemObserver : public QObject {
  Q_OBJECT
 private slots:
  void toJson();
};

void TestItemObserver::toJson() {
  const qreal width_1 = 122;
  const qreal height_1 = width_1;

  auto parent_item_1 = new QQuickItem();
  parent_item_1->setX(55);
  parent_item_1->setY(65);
  parent_item_1->setHeight(width_1);
  parent_item_1->setWidth(height_1);

  auto circle = new CircleItem();
  circle->setWidth(width_1);
  circle->setHeight(height_1);
  circle->setColor("red");
  circle->setOpacity(0.7);
  circle->setParentItem(parent_item_1);
  parent_item_1->setProperty("item", QVariant::fromValue<CircleItem*>(circle));

  ItemObserver item_observer(parent_item_1);

  auto fadeInAnimation = QSharedPointer<FadeIn>(new FadeIn());
  auto fadeOutAnimation = QSharedPointer<FadeOut>(new FadeOut());
  fadeOutAnimation->setStartTime(2.2);
  fadeOutAnimation->setDuration(1.5);

  item_observer.addAnimation(fadeInAnimation);
  item_observer.addAnimation(fadeOutAnimation);

  QJsonObject expected_json = item_observer.abstractitem()->toJson();
  QJsonObject animation_json_0 = fadeInAnimation->toJson();
  QJsonObject animation_json_1 = fadeOutAnimation->toJson();

  expected_json["animation_0"] = animation_json_0;
  expected_json["animation_1"] = animation_json_1;

  QCOMPARE(expected_json, item_observer.toJson());
}

QTEST_MAIN(TestItemObserver)
#include "tst_itemobserver.moc"
