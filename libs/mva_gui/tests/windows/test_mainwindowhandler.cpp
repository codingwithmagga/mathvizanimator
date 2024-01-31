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

#include "mainwindowhandler.h"

class TestMainWindowHandler : public QObject {
    Q_OBJECT

  private slots:
    void changedPropertyTests();
    void unchangedPropertyTests();

    void snapshotRequested();
    void renderingRequested();

    void newProjectRequested();
    void saveProjectRequested();
    void loadProjectRequested();

    void addItemTest();

    void removeCurrentItemRequested();
    void itemClickedByUser();

    void updateProjectSettingsAsVariant();
    void updateProjectSettingsAsInt();

    void updateProjectSettingsAsVariantWrongSize();
    void updateProjectSettingsAsIntWrongSize();

  private:
    void setMainWindowProperties(MainWindowHandler* main_window_handler);
    void checkProperties(const MainWindowHandler& main_window_handler);

    QList<QSharedPointer<QSignalSpy>> createPropertySignalSpies(const MainWindowHandler& main_window_handler);

    const qint32 m_width = 1000;
    const qint32 m_height = 800;
    const qint32 m_fps = 12;
    const qint32 m_video_length = 8;
};

void TestMainWindowHandler::changedPropertyTests()
{
    MainWindowHandler main_window_handler;
    const auto spyList = createPropertySignalSpies(main_window_handler);

    setMainWindowProperties(&main_window_handler);

    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 1);
    }
    checkProperties(main_window_handler);
}

void TestMainWindowHandler::unchangedPropertyTests()
{
    MainWindowHandler main_window_handler;
    QList<int> project_settings { m_width, m_height, m_fps, m_video_length };
    main_window_handler.updateProjectSettings(project_settings);
    const auto spyList = createPropertySignalSpies(main_window_handler);

    setMainWindowProperties(&main_window_handler);

    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 0);
    }
}

void TestMainWindowHandler::snapshotRequested()
{
    MainWindowHandler main_window_handler;
    QSignalSpy snapshotRequestedSpy(&main_window_handler, &MainWindowHandler::snapshotRequested);

    QUrl test_snapshot_file_url("file://some_path/video_file.mp4");
    QFileInfo test_snapshot_file_info(test_snapshot_file_url.toLocalFile());
    connect(&main_window_handler, &MainWindowHandler::renderingRequested, this,
        [&test_snapshot_file_info](const QFileInfo& new_snapshot_file_info) {
            QCOMPARE(test_snapshot_file_info.absoluteFilePath(), new_snapshot_file_info.absoluteFilePath());
        });

    main_window_handler.snapshot(QVariant(test_snapshot_file_url));

    QCOMPARE(snapshotRequestedSpy.count(), 1);
}

void TestMainWindowHandler::renderingRequested()
{
    MainWindowHandler main_window_handler;
    QSignalSpy renderRequestedSpy(&main_window_handler, &MainWindowHandler::renderingRequested);

    QUrl test_video_file_url("file://some_path/video_file.mp4");
    QFileInfo test_video_file_info(test_video_file_url.toLocalFile());
    connect(&main_window_handler, &MainWindowHandler::renderingRequested, this,
        [&test_video_file_info](const QFileInfo& new_video_file_info) {
            QCOMPARE(test_video_file_info.absoluteFilePath(), new_video_file_info.absoluteFilePath());
        });

    main_window_handler.render(QVariant(test_video_file_url));

    QCOMPARE(renderRequestedSpy.count(), 1);
}

void TestMainWindowHandler::newProjectRequested()
{
    MainWindowHandler main_window_handler;
    QSignalSpy newProjectRequestedSpy(&main_window_handler, &MainWindowHandler::newProjectRequested);

    main_window_handler.newProject();

    QCOMPARE(newProjectRequestedSpy.count(), 1);
}

void TestMainWindowHandler::saveProjectRequested()
{
    MainWindowHandler main_window_handler;
    QUrl test_save_file_url("file://some_path/test_file.json");
    QFileInfo test_save_file_info(test_save_file_url.toLocalFile());
    QSignalSpy saveProjectRequestedSpy(&main_window_handler, &MainWindowHandler::saveProjectRequested);

    connect(&main_window_handler, &MainWindowHandler::saveProjectRequested, this,
        [&test_save_file_info](const QFileInfo& signal_save_file_info) {
            QCOMPARE(test_save_file_info.absoluteFilePath(), signal_save_file_info.absoluteFilePath());
        });

    main_window_handler.saveProject(QVariant(test_save_file_url));

    QCOMPARE(saveProjectRequestedSpy.count(), 1);
}

void TestMainWindowHandler::loadProjectRequested()
{
    MainWindowHandler main_window_handler;
    QUrl test_load_file_url("file://some_path/test_file.json");
    QFileInfo test_load_file_info(test_load_file_url.toLocalFile());
    QSignalSpy loadProjectRequestedSpy(&main_window_handler, &MainWindowHandler::loadProjectRequested);

    connect(&main_window_handler, &MainWindowHandler::loadProjectRequested, this,
        [&test_load_file_info](const QFileInfo& signal_load_file_info) {
            QCOMPARE(test_load_file_info.absoluteFilePath(), signal_load_file_info.absoluteFilePath());
        });

    main_window_handler.loadProject(QVariant(test_load_file_url));

    QCOMPARE(loadProjectRequestedSpy.count(), 1);
}

void TestMainWindowHandler::addItemTest()
{
    MainWindowHandler main_window_handler;
    auto test_item = new BasicItem;
    QSignalSpy itemAddedSignalSpy(&main_window_handler, &MainWindowHandler::itemAdded);
    QSignalSpy itemClickedSignalSpy(&main_window_handler, &MainWindowHandler::itemClicked);
    QSignalSpy addAnimationSignalSpy(&main_window_handler, &MainWindowHandler::addAnimationSignal);

    main_window_handler.addItem(test_item);
    emit test_item->clicked("name");
    emit test_item->animationAdded("name", "type", 0.0, 1.0);
    test_item->deleteLater();

    QCOMPARE(itemAddedSignalSpy.count(), 1);
    QCOMPARE(itemClickedSignalSpy.count(), 1);
    QCOMPARE(addAnimationSignalSpy.count(), 1);
}

void TestMainWindowHandler::removeCurrentItemRequested()
{
    MainWindowHandler main_window_handler;
    QSignalSpy removeCurrentItemRequestedSpy(&main_window_handler, &MainWindowHandler::removeCurrentItemRequested);

    main_window_handler.removeCurrentItem();

    QCOMPARE(removeCurrentItemRequestedSpy.count(), 1);
}

void TestMainWindowHandler::itemClickedByUser()
{
    MainWindowHandler main_window_handler;
    const QString item_name = "Name of item";
    QSignalSpy itemClickedSpy(&main_window_handler, &MainWindowHandler::itemClicked);
    connect(&main_window_handler, &MainWindowHandler::itemClicked, this,
        [&item_name](const QString& item_name_signal) { QCOMPARE(item_name, item_name_signal); });

    main_window_handler.itemClickedByUser(item_name);

    QCOMPARE(itemClickedSpy.count(), 1);
}

void TestMainWindowHandler::updateProjectSettingsAsVariant()
{
    MainWindowHandler main_window_handler;
    const auto spyList = createPropertySignalSpies(main_window_handler);
    QVariantList project_settings { m_width, m_height, m_fps, m_video_length };

    main_window_handler.updateProjectSettings(project_settings);

    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 1);
    }
    checkProperties(main_window_handler);
}

void TestMainWindowHandler::updateProjectSettingsAsInt()
{
    MainWindowHandler main_window_handler;
    const auto spyList = createPropertySignalSpies(main_window_handler);
    QList<qint32> project_settings { m_width, m_height, m_fps, m_video_length };

    main_window_handler.updateProjectSettings(project_settings);

    checkProperties(main_window_handler);
    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 1);
    }
}

void TestMainWindowHandler::updateProjectSettingsAsVariantWrongSize()
{
    MainWindowHandler main_window_handler;
    const auto spyList = createPropertySignalSpies(main_window_handler);
    QVariantList project_settings { m_width, m_height, m_fps };

    main_window_handler.updateProjectSettings(project_settings);

    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 0);
    }
}

void TestMainWindowHandler::updateProjectSettingsAsIntWrongSize()
{
    MainWindowHandler main_window_handler;
    const auto spyList = createPropertySignalSpies(main_window_handler);
    QList<qint32> project_settings { m_width, m_fps, m_video_length };

    main_window_handler.updateProjectSettings(project_settings);

    for (const auto& spy : spyList) {
        QCOMPARE(spy->count(), 0);
    }
}

void TestMainWindowHandler::setMainWindowProperties(MainWindowHandler* main_window_handler)
{
    main_window_handler->setPixelWidth(m_width);
    main_window_handler->setPixelHeight(m_height);
    main_window_handler->setFPS(m_fps);
    main_window_handler->setVideoLength(m_video_length);
}

void TestMainWindowHandler::checkProperties(const MainWindowHandler& main_window_handler)
{
    QCOMPARE(main_window_handler.pixelWidth(), m_width);
    QCOMPARE(main_window_handler.pixelHeight(), m_height);
    QCOMPARE(main_window_handler.fps(), m_fps);
    QCOMPARE(main_window_handler.videoLength(), m_video_length);
}

QList<QSharedPointer<QSignalSpy>> TestMainWindowHandler::createPropertySignalSpies(
    const MainWindowHandler& main_window_handler)
{
    QList<QSharedPointer<QSignalSpy>> spy_list;

    spy_list.append(
        QSharedPointer<QSignalSpy>(new QSignalSpy(&main_window_handler, &MainWindowHandler::pixelWidthChanged)));
    spy_list.append(
        QSharedPointer<QSignalSpy>(new QSignalSpy(&main_window_handler, &MainWindowHandler::pixelHeightChanged)));
    spy_list.append(QSharedPointer<QSignalSpy>(new QSignalSpy(&main_window_handler, &MainWindowHandler::fpsChanged)));
    spy_list.append(
        QSharedPointer<QSignalSpy>(new QSignalSpy(&main_window_handler, &MainWindowHandler::videoLengthChanged)));

    return spy_list;
}

QTEST_MAIN(TestMainWindowHandler)
#include "test_mainwindowhandler.moc"
