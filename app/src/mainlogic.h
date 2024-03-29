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

#ifndef APP_SRC_MAINLOGIC_H_
#define APP_SRC_MAINLOGIC_H_

#include <QLoggingCategory>
#include <QObject>
#include <QQmlApplicationEngine>

#include "itemhandler.h"
#include "mainwindowhandler.h"
#include "renderer.h"
#include "savefilehandler.h"

Q_DECLARE_LOGGING_CATEGORY(mainlogic)

class MainLogic : public QObject {
    Q_OBJECT

  public:
    explicit MainLogic(QObject* parent = Q_NULLPTR);

    void initEngine(QQmlApplicationEngine* const engine);
    void connectEngine();

  private slots:
    void createSnapshot(const QFileInfo& snapshot_file_info);
    void renderVideo(const QFileInfo& video_file_info);

    void saveProject(const QFileInfo& save_file_info);
    void loadProject(const QFileInfo& load_file_info);

    void projectSizeChanged();

    void uiTimeChanged(const qreal time);
    void renderingVideoFinished();

  private:
    QJsonObject projectSettingsJson() const;
    void setProjectSettings(const QJsonObject& json);

    QQmlApplicationEngine* m_qml_engine;
    QObject* m_qml_creation_area;

    qreal m_current_time = 0.0;

    MainWindowHandler m_mainwindowhandler;

    SaveFileHandler m_savefilehandler;
    Renderer m_renderer;
    ItemHandler m_itemhandler;
};

#endif // APP_SRC_MAINLOGIC_H_
