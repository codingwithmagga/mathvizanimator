#ifndef SAVEFILEHANDLER_H
#define SAVEFILEHANDLER_H

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

class SaveFileHandler
{
public:
    SaveFileHandler() = default;

    QDir saveDir() const;
    void setSaveDir(const QDir &new_save_dir);

    bool saveJSON(const QString &file_name, const QJsonObject json_data) const;
    QJsonDocument loadJSON(const QFileInfo &fileinfo) const;

private:
    QSaveFile prepareSaveFile(const QString &name, const QString &extension) const;

    QString prepareFileName(const QString &extension, const QFileInfo &file_info) const;

    QDir m_save_dir = QDir::home();
};

#endif // SAVEFILEHANDLER_H
