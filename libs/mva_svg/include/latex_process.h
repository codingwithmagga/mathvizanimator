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

#ifndef LIBS_MVA_SVG_INCLUDE_LATEX_PROCESS_H_
#define LIBS_MVA_SVG_INCLUDE_LATEX_PROCESS_H_

#include <QFileInfo>
#include <QProcess>

/**
 * @brief The LaTeXProcess class handles the execution of LaTeX typesetting process to a DVI file.
 *
 * This class inherits from QProcess and provides functionalities to compile LaTeX files
 * into DVI format and clean up auxiliary files generated during the compilation process.
 * The class should be used as follows. First specifiy the necessary LaTeX file in the constructor of the class. When
 * the conversion should be started call the QProcess::start() method. To get informed about the result of the
 * conversion process, connect to the processFailed() and processFinished() signal of this class. The DVI File will be
 * created in the same folder as the given LaTeX file having the same base name. After the conversion process the object
 * can be destroyed.
 */
class LaTeXProcess : public QProcess {
    Q_OBJECT

  public:
    /**
     * @brief Constructs a LaTeXProcess object.
     * @param latex_file The QFileInfo object representing the LaTeX file to be compiled.
     * @param parent Optional pointer to the parent QObject.
     */
    explicit LaTeXProcess(const QFileInfo& latex_file, QObject* parent = nullptr);

    /**
     * @brief Performs cleanup by removing auxiliary files generated during the LaTeX compilation process.
     */
    void cleanup() const;

    /**
     * @brief Returns the QFileInfo object representing the LaTeX file.
     * @return QFileInfo object representing the LaTeX file.
     */
    QFileInfo latexFile() const { return m_latex_file; }

  signals:
    /**
     * @brief Signal emitted when the LaTeX compilation process fails.
     * @param latex_file The QFileInfo object representing the LaTeX file that failed to compile.
     */
    void processFailed(const QFileInfo& latex_file);

    /**
     * @brief Signal emitted when the LaTeX compilation process successfully finishes.
     * @param dvi_file The QFileInfo object representing the generated DVI file.
     */
    void processFinished(const QFileInfo& dvi_file);

  private:
    /**
     * @brief Slot invoked when the LaTeX compilation process finishes.
     * @param exitCode The exit code of the process.
     * @param exitStatus The exit status of the process.
     */
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QFileInfo m_latex_file;
};

#endif // LIBS_MVA_SVG_INCLUDE_LATEX_PROCESS_H_
