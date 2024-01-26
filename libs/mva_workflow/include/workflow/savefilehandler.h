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

#ifndef LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_SAVEFILEHANDLER_H_
#define LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_SAVEFILEHANDLER_H_

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

class SaveFileHandler {
public:
    SaveFileHandler() = default;

    QDir saveDir() const;
    void setSaveDir(const QDir& new_save_dir);

    bool saveJSON(const QString& file_name, const QJsonObject json_data) const;
    QJsonDocument loadJSON(const QFileInfo& fileinfo) const;

private:
    QSaveFile prepareSaveFile(const QString& name,
        const QString& extension) const;

    QString prepareFileName(const QString& extension,
        const QFileInfo& file_info) const;

    QDir m_save_dir = QDir::home();
};

#endif // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_SAVEFILEHANDLER_H_
