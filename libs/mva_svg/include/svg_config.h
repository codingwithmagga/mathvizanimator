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

/**
 * @brief The SVGConfig class manages the configuration related to SVG files.
 *
 * This class provides functionalities to set and retrieve the directory where SVG files are stored,
 * as well as a method to generate a hash value for a given text. It is created as a Singleton class so that
 * the directory for the SVG files, as well as the used hash algorithm are the same through the whole
 * library/application. You cannot create an Object of this class, but you can get an Instance of it through the
 * function SVGConfig::getInstance().
 */
class SVGConfig {
  public:
    /**
     * @brief Returns the singleton instance of SVGConfig.
     * @return Reference to the singleton instance of SVGConfig.
     */
    static SVGConfig& getInstance()
    {
        static SVGConfig instance;
        return instance;
    }

    /**
     * @brief Sets the directory for SVG files.
     * @param svg_dir The directory where SVG files will be stored.
     */
    void setSVGDir(const QDir& svg_dir)
    {
        m_svg_dir = svg_dir;

        if (!m_svg_dir.exists()) {
            m_svg_dir.mkpath(".");
        }
    }

    /**
     * @brief Returns the directory where SVG files are stored.
     * @return The directory where SVG files are stored.
     */
    QDir svgDir() { return m_svg_dir; }

    /**
     * @brief Generates a hash value for the given text using MD5 algorithm.
     * @param text The text for which the hash value will be generated.
     * @return The hash value generated for the text.
     */
    static QString hash(const QString& text)
    {
        return QString(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5).toHex());
    }

  private:
    SVGConfig() = default;
    ~SVGConfig() = default;
    SVGConfig(const SVGConfig&) = delete;
    SVGConfig& operator=(const SVGConfig&) = delete;

    QDir m_svg_dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
};

#endif // LIBS_MVA_SVG_INCLUDE_SVG_CONFIG_H_
