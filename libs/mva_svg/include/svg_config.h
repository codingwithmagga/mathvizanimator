#ifndef SVG_CONFIG_H
#define SVG_CONFIG_H

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

#endif // SVG_CONFIG_H
