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

#ifndef LIBS_MVA_GUI_INCLUDE_ITEMS_TEXTITEM_H_
#define LIBS_MVA_GUI_INCLUDE_ITEMS_TEXTITEM_H_

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QStandardPaths>

#include "abstractitem.h"

class TextItem : public AbstractItem {
  Q_OBJECT
  QML_ELEMENT

  // TODO(codingwithmagga): Relocate to a SvgHandler class or LatexHandler or
  // similar
  Q_PROPERTY(QString latexSource READ getLatexSource WRITE setLatexSource NOTIFY
                 latexSourceChanged)

  Q_PROPERTY(
      QString svgFile READ getSvgFile WRITE setSvgFile NOTIFY svgFileChanged)
  Q_PROPERTY(qreal scaleText READ getScaleText WRITE setScaleText NOTIFY
                 scaleTextChanged)

 public:
  explicit TextItem(QQuickItem* parent = nullptr);

  QString getSvgFile() const { return m_svg_file.absoluteFilePath(); }
  void setSvgFile(const QFileInfo& newSvgFile);
  void setSvgFile(const QString& newSvgFile);

  void paint(QPainter* painter) override;

  QString getLatexSource() const;
  // TODO(codingwithmagga): Relocate to a SvgHandler class or LatexHandler or
  // similar
  void setLatexSource(const QString& newLatexSource);

  qreal getScaleText() const;
  void setScaleText(qreal newScaleText);

  EditableProperties editableProperties() const override;

 signals:
  void latexSourceChanged(const QString& new_latex_source);
  void svgFileChanged(const QFileInfo& new_svg_file);
  void scaleTextChanged(const qreal new_scale_text);

 private:
  QFileInfo m_svg_file = QFileInfo("://templates/placeholder.svg");
  QString m_latex_source;
  qreal m_scale_text = 1.0;

  // TODO(codingwithmagga): Relocate to a SvgHandler class or LatexHandler or
  // similar
  QDir m_svg_location;
  QString m_latex_path;
  QString m_dvisvgm_path;
};

#endif  // LIBS_MVA_GUI_INCLUDE_ITEMS_TEXTITEM_H_
