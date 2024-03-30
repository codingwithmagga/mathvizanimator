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

#ifndef LIBS_MVA_SVG_INCLUDE_SVG_CONFIG_H_
#define LIBS_MVA_SVG_INCLUDE_SVG_CONFIG_H_

#include <QCryptographicHash>
#include <QDir>
#include <QStandardPaths>

class SVGConfig {
  public:
    static SVGConfig& getInstance()
    {
        static SVGConfig instance;
        return instance;
    }

    void setSVGDir(const QDir& svg_dir)
    {
        m_svg_dir = svg_dir;

        if (!m_svg_dir.exists()) {
            m_svg_dir.mkpath(".");
        }
    }

    QDir svgDir() { return m_svg_dir; }

    static QString hash(const QString& string)
    {
        return QString(QCryptographicHash::hash(string.toUtf8(), QCryptographicHash::Md5).toHex());
    }

  private:
    SVGConfig() = default;
    ~SVGConfig() = default;
    SVGConfig(const SVGConfig&) = delete;
    SVGConfig& operator=(const SVGConfig&) = delete;

    QDir m_svg_dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
};

#endif // LIBS_MVA_SVG_INCLUDE_SVG_CONFIG_H_
