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

#include "main.h"
#include "test_helper_functions.h"

class UserInteractionTest : public QObject {
    Q_OBJECT

  private slots:
    void init();

    void moveItemChangeTime();

    void cleanup();

  private:
    SetupMain::SetupObjects m_app_objects;
    QSharedPointer<TestHelperFunctions> m_helper_functions;
};

void UserInteractionTest::init()
{
    m_app_objects = SetupMain::setupApp();

    m_helper_functions = QSharedPointer<TestHelperFunctions>(new TestHelperFunctions(m_app_objects.engine));
}

void UserInteractionTest::moveItemChangeTime()
{
    const qint32 item_number = 0;
    m_helper_functions->dragAndDropCurrentItem(QPoint(100, 100));
    const auto basic_item = m_helper_functions->getQuickItem(item_number);
    const auto previous_x = basic_item->x();
    const auto previous_y = basic_item->y();

    m_helper_functions->moveItem(basic_item, QPoint(50, 15));
    m_helper_functions->changeTime(1.3);

    QCOMPARE(basic_item->opacity(), 1.0);
    QCOMPARE_NE(basic_item->x(), previous_x);
    QCOMPARE_NE(basic_item->y(), previous_y);
}

void UserInteractionTest::cleanup()
{
    m_helper_functions.clear();

    m_app_objects.engine.clear();
    m_app_objects.mainlogic.clear();
}

QTEST_MAIN(UserInteractionTest)
#include "tst_user_interaction.moc"
