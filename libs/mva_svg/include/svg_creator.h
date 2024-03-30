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

#ifndef SVG_CREATOR_H
#define SVG_CREATOR_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QProcess>

#include "dvisvgm_process.h"
#include "latex_process.h"

class SVGCreator : public QObject {
    Q_OBJECT
  public:
    explicit SVGCreator(QObject* parent = nullptr);

    void svgFromLaTeX(const QString& latex);

  signals:
    void svgCreated(const QFileInfo& svg_file);
    void svgCreationFailed();

  private slots:
    void latexProcessFinished(const QFileInfo& dvi_file);
    void dvisvgmProcessFinished(const QFileInfo& svg_file);

  private:
    QFileInfo prepareLaTeXFile(const QString& latex);

    QMap<QString, QSharedPointer<LaTeXProcess>> m_latex_process_map;
    QMap<QString, QSharedPointer<DvisvgmProcess>> m_dvisvgm_process_map;
};

#endif // SVG_CREATOR_H
