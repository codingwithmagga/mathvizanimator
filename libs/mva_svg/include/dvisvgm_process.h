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

/**
 * @brief The DvisvgmProcess class handles the conversion of DVI files to SVG format using the dvisvgm utility.
 *
 * This class inherits from QProcess and emits signals upon successful completion or failure of the conversion process.
 * The class should be used as follows. First specify the necessary DVI file in the constructor of the class. When the
 * conversion should be started call the QProcess::start() method. To get informed about the result of the conversion
 * process, connect to the processFailed() and processFinished() signal of this class. The SVG File will be created in
 * the same folder as the given DVI file having the same base name. After the conversion process the object can be
 * destroyed.
 */
class DvisvgmProcess : public QProcess {
    Q_OBJECT

  public:
    /**
     * @brief Constructs a DvisvgmProcess object.
     * @param dvi_file The QFileInfo object representing the DVI file to be converted.
     * @param parent Optional pointer to the parent QObject.
     */
    explicit DvisvgmProcess(const QFileInfo& dvi_file, QObject* parent = nullptr);

    /**
     * @brief Returns the QFileInfo object representing the DVI file.
     * @return QFileInfo object representing the DVI file.
     */
    QFileInfo dviFile() const { return m_dvi_file; }

  signals:
    /**
     * @brief Signal emitted when the conversion process fails.
     * @param dvi_file The QFileInfo object representing the DVI file that failed to convert.
     */
    void processFailed(const QFileInfo& dvi_file);

    /**
     * @brief Signal emitted when the conversion process successfully finishes.
     * @param svg_file The QFileInfo object representing the generated SVG file.
     */
    void processFinished(const QFileInfo& svg_file);

  private:
    /**
     * @brief Slot invoked when the conversion process finishes.
     * @param exitCode The exit code of the process.
     * @param exitStatus The exit status of the process.
     */
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QFileInfo m_dvi_file;
};

#endif // LIBS_MVA_SVG_INCLUDE_DVISVGM_PROCESS_H_
