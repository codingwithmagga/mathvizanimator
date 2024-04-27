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

#include "textitem.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QLoggingCategory>
#include <QPainter>
#include <QPen>
#include <QProcess>
#include <QSvgRenderer>

#include "svg_config.h"
#include "svg_creator.h"

Q_LOGGING_CATEGORY(text_item, "cwa.mva.gui.text_item")

TextItem::TextItem(BasicItem* parent)
    : AbstractItem { "qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml", parent }
    , m_svg_location(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
{
    Q_INIT_RESOURCE(mva_gui_resources);

    if (!m_svg_location.exists()) {
        m_svg_location.mkpath(".");
    }

    // FallbackLocation
    if (!QFileInfo(m_svg_location.absolutePath()).isWritable()) {
        m_svg_location.setPath(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
        if (!m_svg_location.exists()) {
            m_svg_location.mkpath(".");
        }
    }

    SVGConfig::getInstance().setSVGDir(m_svg_location);

    m_latex_path = QStandardPaths::findExecutable("latex");
    m_dvisvgm_path = QStandardPaths::findExecutable("dvisvgm");

    // TODO(codingwithmagga): This needs to be in it's own class
    if (m_latex_path.isEmpty() || m_dvisvgm_path.isEmpty()) {
        qCritical() << "Latex or dvisvgm not found!";
    }

    connect(&m_svg_creator, &SVGCreator::svgCreated, this, &TextItem::svgCreationFinished);
    connect(&m_svg_creator, &SVGCreator::svgCreationFailed, this,
        []() { qCWarning(text_item) << "Svg creation from LaTeX failed."; });
}

void TextItem::setSvgFile(const QFileInfo& newSvgFile)
{
    m_svg_file = newSvgFile;
    update();
    emit svgFileChanged(newSvgFile);
}

void TextItem::setSvgFile(const QString& newSvgFile) { setSvgFile(QFileInfo(newSvgFile)); }

void TextItem::paint(QPainter* painter)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing, true);

    QSvgRenderer renderer(m_svg_file.absoluteFilePath());

    // Add 10 percent (1.1) additional space, because otherwise some text is cut
    // off a little bit, for example the bottom of a small 't'
    const auto add_space = 1.1;

    const auto size = renderer.defaultSize() * m_scale_text * add_space;
    parentItem()->setWidth(size.width());
    parentItem()->setHeight(size.height());
    setWidth(size.width());
    setHeight(size.height());

    renderer.render(painter, QRect(QPoint(x(), y()), size / add_space));
    painter->restore();
}

QString TextItem::latexSource() const { return m_latex_source; }

void TextItem::setLatexSource(const QString& newLatexSource)
{
    QFile latexTemplateFile("://templates/template.tex");
    if (!latexTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open: " << latexTemplateFile.fileName();
        return;
    }

    QString latexTemplate = latexTemplateFile.readAll();
    latexTemplateFile.close();

    latexTemplate.replace("%PLACEHOLDER%", newLatexSource);

    m_svg_creator.svgFromLaTeX(latexTemplate);

    m_latex_source = newLatexSource;
    emit latexSourceChanged(newLatexSource);
}

qreal TextItem::scaleText() const { return m_scale_text; }

void TextItem::setScaleText(qreal newScaleText)
{
    if (qFuzzyCompare(m_scale_text, newScaleText)) {
        return;
    }

    m_scale_text = newScaleText;
    emit scaleTextChanged(m_scale_text);
}

QStringList TextItem::editableProperties() const
{
    auto editableProperties = AbstractItem::editableProperties();

    editableProperties.append("latexSource");
    editableProperties.append("scaleText");

    return editableProperties;
}

QStringList TextItem::editablePropertiesParent() const
{
    auto editableProperties = AbstractItem::editablePropertiesParent();

    editableProperties.removeOne("width");
    editableProperties.removeOne("height");

    return editableProperties;
}

void TextItem::svgCreationFinished(const QFileInfo& svg_file) { setSvgFile(svg_file); }

void TextItem::removeUnusedLatexFiles(const QString& hash)
{
    QList<QString> file_appendices { ".aux", ".dvi", ".log" };

    for (const auto& appendix : file_appendices) {
        QFile::remove(m_svg_location.absoluteFilePath(hash + appendix));
    }
}
