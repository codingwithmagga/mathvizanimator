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

#include <QSignalSpy>
#include <QTest>

#include "main.h"
#include "test_helper_functions.h"

class AnimationIntegrationTest : public QObject {
    Q_OBJECT

  private slots:
    void init();

    void addAnimation();
    void removeAnimation();

    void setUserTime_data();
    void setUserTime();

    void cleanup();

  private:
    void removeAnimationFromCurrentItem(const qint32 animation_number);

    SetupMain::SetupObjects m_app_objects;
    QSharedPointer<TestHelperFunctions> m_helper_functions;

    QSharedPointer<QSignalSpy> m_finishedVideoRenderingSpy;
};

void AnimationIntegrationTest::init()
{
    m_app_objects = SetupMain::setupApp();

    m_helper_functions = QSharedPointer<TestHelperFunctions>(new TestHelperFunctions(m_app_objects.engine));
}

void AnimationIntegrationTest::addAnimation()
{
    const qint32 item_number = 0;
    const qreal start_time = 1.2;
    const qreal duration = 2.3;
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));

    m_helper_functions->addAnimationToItem(item_number, start_time, duration);
    const auto item_observer = m_helper_functions->getItemObserver(item_number);
    const auto animations = item_observer->animations();

    QCOMPARE(animations.size(), 1);
    QCOMPARE(animations[item_number]->startTime(), start_time);
    QCOMPARE(animations[item_number]->duration(), duration);
}

void AnimationIntegrationTest::removeAnimation()
{
    const qint32 item_number = 0;
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    m_helper_functions->clickItem(m_helper_functions->getQuickItem(item_number));

    m_helper_functions->addAnimationToItem(item_number, 0.0, 1.0);
    removeAnimationFromCurrentItem(0);
    const auto item_observer = m_helper_functions->getItemObserver(item_number);
    const auto animations = item_observer->animations();

    QCOMPARE(animations.size(), 0);
}

void AnimationIntegrationTest::setUserTime_data()
{
    QTest::addColumn<qreal>("time");
    QTest::addColumn<qreal>("opacity");

    QTest::newRow("start_project") << 0.0 << 0.0;
    QTest::newRow("fade_in_before_start") << 0.9 << 0.0;
    QTest::newRow("fade_in_start") << 1.0 << 0.0;
    QTest::newRow("fade_in_25_percent") << 1.5 << 0.25;
    QTest::newRow("fade_in_50_percent") << 2.0 << 0.5;
    QTest::newRow("fade_in_75_percent") << 2.5 << 0.75;
    QTest::newRow("fade_in_end") << 3.0 << 1.0;
    QTest::newRow("end_project") << 5.0 << 1.0;
}

void AnimationIntegrationTest::setUserTime()
{
    QFETCH(qreal, time);
    QFETCH(qreal, opacity);

    const qint32 item_number = 0;
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    m_helper_functions->clickItem(m_helper_functions->getQuickItem(item_number));

    m_helper_functions->addAnimationToItem(item_number, 1.0, 2.0);
    const auto item_observer = m_helper_functions->getItemObserver(item_number);
    const auto item = item_observer->abstractitem();
    item_observer->updateItemProperty("opacity", 0.0);

    const QList<qreal> initial_time_list = { 0.0, 0.5, 1.2, 1.7, 2.4, 2.9, 3.5, 4.0 };

    auto time_slider = m_helper_functions->getChild<QObject*>("MVATimeSlider");
    for (const auto initial_time : initial_time_list) {
        time_slider->setProperty("value", initial_time);
        time_slider->setProperty("value", time);
        QCOMPARE(item->opacity(), opacity);
    }
}

void AnimationIntegrationTest::cleanup()
{
    m_helper_functions.clear();

    m_app_objects.engine.clear();
    m_app_objects.mainlogic.clear();
}

void AnimationIntegrationTest::removeAnimationFromCurrentItem(const qint32 animation_number)
{
    auto item_bar = m_helper_functions->getChild<QObject*>("MVAItemBar");
    item_bar->setProperty("currentIndex", 1);
    QVERIFY(QTest::qWaitFor([&]() { return item_bar->property("visible").toBool(); }));

    auto animation_item = m_helper_functions->getAnimationItem(animation_number);
    m_helper_functions->clickItem(animation_item, Qt::MouseButton::RightButton);

    auto animation_menu_delete = animation_item->findChild<QObject*>("MVAAnimationMenuDelete");
    QVERIFY(QTest::qWaitFor([&]() { return animation_menu_delete->property("visible").toBool(); }));
    QMetaObject::invokeMethod(animation_menu_delete, "simulateClicked");
}

QTEST_MAIN(AnimationIntegrationTest)
#include "tst_animation.moc"
