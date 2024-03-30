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

#include "svg_creator.h"

#include <QCryptographicHash>
#include <QProcess>
#include <QStandardPaths>

#include "dvisvgm_process.h"
#include "latex_process.h"
#include "svg_config.h"

SVGCreator::SVGCreator(QObject* parent)
    : QObject { parent }
{
}

void SVGCreator::svgFromLaTeX(const QString& latex)
{
    const auto hash = SVGConfig::hash(latex);

    QFileInfo svgFile(SVGConfig::getInstance().svgDir().absoluteFilePath(hash + ".svg"));
    if (svgFile.exists()) {
        emit svgCreated(svgFile);
        return;
    }

    const auto latex_process = QSharedPointer<LaTeXProcess>(new LaTeXProcess(prepareLaTeXFile(latex)));
    m_latex_process_map.insert(hash, latex_process);

    connect(latex_process.data(), &LaTeXProcess::processFinished, this,
        [&](const QFileInfo& dvi_file) { latexProcessFinished(dvi_file); });
    connect(latex_process.data(), &LaTeXProcess::processFailed, this, &SVGCreator::svgCreationFailed);
    latex_process->start();
}

void SVGCreator::latexProcessFinished(const QFileInfo& dvi_file)
{
    const auto hash = dvi_file.baseName();

    const auto dvisvgm_process = QSharedPointer<DvisvgmProcess>(new DvisvgmProcess(dvi_file));
    m_dvisvgm_process_map.insert(hash, dvisvgm_process);

    connect(dvisvgm_process.data(), &DvisvgmProcess::processFinished, this,
        [&](const QFileInfo& dvi_file) { dvisvgmProcessFinished(dvi_file); });
    connect(dvisvgm_process.data(), &DvisvgmProcess::processFailed, this, &SVGCreator::svgCreationFailed);

    dvisvgm_process->start();
}

void SVGCreator::dvisvgmProcessFinished(const QFileInfo& svg_file)
{
    const auto hash = svg_file.baseName();

    m_dvisvgm_process_map.take(hash);
    const auto latex_process = m_latex_process_map.take(hash);
    latex_process->cleanup();

    emit svgCreated(svg_file);
}

QFileInfo SVGCreator::prepareLaTeXFile(const QString& latex)
{
    const auto hash = QString(QCryptographicHash::hash(latex.toUtf8(), QCryptographicHash::Md5).toHex());

    QFile latexFile(SVGConfig::getInstance().svgDir().absoluteFilePath(hash + ".tex"));
    if (!latexFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open: " << latexFile.fileName();
        return QFileInfo();
    }

    QTextStream stream(&latexFile);
    stream << latex;
    latexFile.close();

    return QFileInfo(latexFile.fileName());
}
