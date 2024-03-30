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

#include "dvisvgm_process.h"

#include "svg_config.h"

DvisvgmProcess::DvisvgmProcess(const QFileInfo& dvi_file, QObject* parent)
    : QProcess(parent)
    , m_dvi_file(dvi_file)
{
    setWorkingDirectory(SVGConfig::getInstance().svgDir().absolutePath());
    setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
    setProgram(QStandardPaths::findExecutable("dvisvgm"));
    setArguments(QStringList {} << m_dvi_file.absoluteFilePath() << "-n"
                                << "-o" << m_dvi_file.baseName() + ".svg");

    connect(this, &QProcess::finished, this, &DvisvgmProcess::onFinished);
}

// jscpd:ignore-start
void DvisvgmProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0 || exitStatus != QProcess::ExitStatus::NormalExit) {
        emit processFailed();
        return;
    }
    emit processFinished(QFileInfo(SVGConfig::getInstance().svgDir().absoluteFilePath(m_dvi_file.baseName() + ".svg")));
}
// jscpd:ignore-end
