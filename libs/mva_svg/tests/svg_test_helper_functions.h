/* mathvizanimator
 * Copyright (C) 2024 codingwithmagga
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

#ifndef LIBS_MVA_SVG_TESTS_SVG_TEST_HELPER_FUNCTIONS_H_
#define LIBS_MVA_SVG_TESTS_SVG_TEST_HELPER_FUNCTIONS_H_

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

QString readData(const QString& file_path)
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

} // namespace SVGTestHelperFunctions

#endif // LIBS_MVA_SVG_TESTS_SVG_TEST_HELPER_FUNCTIONS_H_
