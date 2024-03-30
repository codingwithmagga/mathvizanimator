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

#ifndef LIBS_MVA_SVG_INCLUDE_DVISVGM_PROCESS_H_
#define LIBS_MVA_SVG_INCLUDE_DVISVGM_PROCESS_H_

#include <QFileInfo>
#include <QProcess>

class DvisvgmProcess : public QProcess {

    Q_OBJECT

  public:
    DvisvgmProcess(const QFileInfo& dvi_file, QObject* parent = nullptr);

    QFileInfo dviFile() const { return m_dvi_file; }

  signals:
    void processFailed();
    void processFinished(const QFileInfo& svg_file);

  private:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QFileInfo m_dvi_file;
};

#endif // LIBS_MVA_SVG_INCLUDE_DVISVGM_PROCESS_H_
