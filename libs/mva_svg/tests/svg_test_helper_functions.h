#ifndef SVG_TEST_HELPER_FUNCTIONS_H
#define SVG_TEST_HELPER_FUNCTIONS_H

#include <QFile>
#include <QFileInfo>
#include <QString>

#include "svg_config.h"

namespace SVGTestHelperFunctions {

QFileInfo createLocalFile(const QString& resource_file_path)
{
    QFile resource_file(resource_file_path);
    const QString copy_file_name
        = SVGConfig::getInstance().svgDir().absoluteFilePath(QFileInfo(resource_file.fileName()).baseName())
        + "_copy.tex";

    if (QFile::exists(copy_file_name)) {
        QFile::remove(copy_file_name);
    }
    resource_file.copy(copy_file_name);

    return QFileInfo(copy_file_name);
}

QString readData(QString file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open: " << file.fileName();
        return QString();
    }

    QString data = file.readAll();
    file.close();

    return data;
}

}

#endif // SVG_TEST_HELPER_FUNCTIONS_H
