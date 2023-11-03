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

#include "savefilehandler.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

#include "logging.h"

bool SaveFileHandler::saveJSON(const QString &file_name,
                               const QJsonObject json_data) const {
  QSaveFile saveFile = prepareSaveFile(file_name, "json");

  if (!saveFile.open(QIODevice::WriteOnly)) {
    qCWarning(savefilehandler)
        << "Couldn't open save file: " << saveFile.fileName();
    return false;
  }

  saveFile.write(QJsonDocument(json_data).toJson());
  return saveFile.commit();
}

QJsonDocument SaveFileHandler::loadJSON(const QFileInfo &fileinfo) const {
  if (fileinfo.suffix() != "json") {
    qCWarning(savefilehandler) << "Not a json file, check file extension of "
                               << fileinfo.absoluteFilePath();
    return QJsonDocument{};
  }

  QFile loadFile(fileinfo.absoluteFilePath());

  if (!loadFile.open(QIODevice::ReadOnly)) {
    qCWarning(savefilehandler)
        << "Couldn't open save file: " << loadFile.fileName();
    return QJsonDocument{};
  }

  QByteArray saveData = loadFile.readAll();

  return QJsonDocument::fromJson(saveData);
}

QSaveFile SaveFileHandler::prepareSaveFile(const QString &name,
                                           const QString &extension) const {
  QFileInfo savefile_info(m_save_dir.absoluteFilePath(name));
  QString savefile_name = prepareFileName(extension, savefile_info);

  return QSaveFile(savefile_name);
}

QString SaveFileHandler::prepareFileName(const QString &extension,
                                         const QFileInfo &file_info) const {
  QString file_name;
  if (file_info.suffix().compare(extension) == 0) {
    file_name = file_info.absoluteFilePath();
  } else {
    file_name = file_info.absoluteFilePath().append("." + extension);
  }

  return file_name;
}

QDir SaveFileHandler::saveDir() const { return m_save_dir; }

void SaveFileHandler::setSaveDir(const QDir &new_save_dir) {
  m_save_dir = new_save_dir;
}
