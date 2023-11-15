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
#include <QPainter>
#include <QPen>
#include <QProcess>
#include <QSvgRenderer>

TextItem::TextItem(QQuickItem* parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml", parent},
      m_svg_location(
          QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)) {
  Q_INIT_RESOURCE(mva_gui_resources);

  if (!m_svg_location.exists()) {
    m_svg_location.mkpath(".");
  }

  // FallbackLocation
  if (!QFileInfo(m_svg_location.absolutePath()).isWritable()) {
    m_svg_location.setPath(
        QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    if (!m_svg_location.exists()) {
      m_svg_location.mkpath(".");
    }
  }

  m_latex_path = QStandardPaths::findExecutable("latex");
  m_dvisvgm_path = QStandardPaths::findExecutable("dvisvgm");

  // TODO(codingwithmagga): This needs to be in it's own class
  if (m_latex_path.isEmpty() || m_dvisvgm_path.isEmpty()) {
    qCritical() << "Latex or dvisvgm not found!";
  }
}

void TextItem::setSvgFile(const QFileInfo& newSvgFile) {
  m_svg_file = newSvgFile;
  update();
  emit svgFileChanged(newSvgFile);
}

void TextItem::setSvgFile(const QString& newSvgFile) {
  setSvgFile(QFileInfo(newSvgFile));
}

void TextItem::paint(QPainter* painter) {
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

// TODO(codingwithmagga): save everything from editableProperties()
QJsonObject TextItem::toJson() const {
  auto json = AbstractItem::toJson();

  json["item.latexSource"] = m_latex_source;
  json["item.svgFile"] = m_svg_file.absoluteFilePath();

  return json;
}

QString TextItem::getLatexSource() const { return m_latex_source; }

// TODO(codingwithmagga): Refactor this function
void TextItem::setLatexSource(const QString& newLatexSource) {
  QFile latexTemplateFile("://templates/template.tex");
  if (!latexTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical() << "Cannot open: " << latexTemplateFile.fileName();
    return;
  }

  QString latexTemplate = latexTemplateFile.readAll();
  latexTemplateFile.close();

  latexTemplate.replace("%PLACEHOLDER%", newLatexSource);

  const auto hash = QString(
      QCryptographicHash::hash(latexTemplate.toUtf8(), QCryptographicHash::Md5)
          .toHex());

  QFileInfo svgFile(m_svg_location.absoluteFilePath(hash + ".svg"));
  if (svgFile.exists()) {
    m_latex_source = newLatexSource;
    emit latexSourceChanged(newLatexSource);

    setSvgFile(svgFile);
    return;
  }

  QFile latexFile(m_svg_location.absoluteFilePath(hash + ".tex"));
  if (!latexFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qWarning() << "Cannot open: " << latexFile.fileName();
    return;
  }

  QTextStream stream(&latexFile);
  stream << latexTemplate;
  latexFile.close();

  QProcess latexmk_process;
  latexmk_process.setWorkingDirectory(m_svg_location.absolutePath());
  latexmk_process.setProcessChannelMode(
      QProcess::ProcessChannelMode::MergedChannels);
  latexmk_process.start(m_latex_path, QStringList{} << "-output-format=dvi"
                                                    << "-interaction=batchmode"
                                                    << latexFile.fileName());
  if (!latexmk_process.waitForFinished()) {
    qDebug() << "Make failed:" << latexmk_process.errorString();
    return;
  }

  QProcess dvisvgm_process;
  dvisvgm_process.setWorkingDirectory(m_svg_location.absolutePath());
  dvisvgm_process.start(m_dvisvgm_path, QStringList{} << hash + ".dvi"
                                                      << "-n"
                                                      << "-o" << hash + ".svg");
  if (!dvisvgm_process.waitForFinished()) {
    qDebug() << "Make failed:" << dvisvgm_process.errorString();
    return;
  }

  setSvgFile(svgFile);

  m_latex_source = newLatexSource;
  emit latexSourceChanged(newLatexSource);
}

qreal TextItem::getScaleText() const { return m_scale_text; }

void TextItem::setScaleText(qreal newScaleText) {
  if (qFuzzyCompare(m_scale_text, newScaleText)) {
    return;
  }

  m_scale_text = newScaleText;
  emit scaleTextChanged(m_scale_text);
}

AbstractItem::EditableProperties TextItem::editableProperties() const {
  auto abstractList = AbstractItem::editableProperties();
  abstractList.quick_item_properties.removeOne("width");
  abstractList.quick_item_properties.removeOne("height");
  abstractList.abstract_item_properties.removeOne("color");

  abstractList.abstract_item_properties.append("latexSource");
  abstractList.abstract_item_properties.append("scaleText");

  return abstractList;
}
