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

#include "latex_process.h"

#include "svg_config.h"

LaTeXProcess::LaTeXProcess(const QFileInfo& latex_file, QObject* parent)
    : QProcess(parent)
    , m_latex_file(latex_file)
{
    setWorkingDirectory(SVGConfig::getInstance().svgDir().absolutePath());
    setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
    setProgram(QStandardPaths::findExecutable("latex"));
    setArguments(QStringList {} << "-output-format=dvi"
                                << "-interaction=batchmode" << m_latex_file.absoluteFilePath());

    connect(this, &QProcess::finished, this, &LaTeXProcess::onFinished);
}

void LaTeXProcess::cleanup() const
{
    const QList<QString> file_appendices { ".aux", ".dvi", ".log" };
    const auto svg_dir = SVGConfig::getInstance().svgDir();

    for (const auto& appendix : file_appendices) {
        QFile::remove(svg_dir.absoluteFilePath(m_latex_file.baseName() + appendix));
    }
}

// jscpd:ignore-start
void LaTeXProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0 || exitStatus != QProcess::ExitStatus::NormalExit) {
        emit processFailed(m_latex_file);
        return;
    }
    emit processFinished(
        QFileInfo(SVGConfig::getInstance().svgDir().absoluteFilePath(m_latex_file.baseName() + ".dvi")));
}
// jscpd:ignore-start
