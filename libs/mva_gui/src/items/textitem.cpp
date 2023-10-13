#include "textitem.h"

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QPainter>
#include <QPen>
#include <QProcess>
#include <QSvgRenderer>

TextItem::TextItem(QQuickItem* parent)
    : AbstractItem { "qrc:/qt/qml/cwa/mva/gui/qml/items/MVAText.qml", parent }
{
    Q_INIT_RESOURCE(mva_gui_resources);
}

void TextItem::setSvgFile(const QFileInfo& newSvgFile)
{
    m_svg_file = newSvgFile;
    update();
    emit svgFileChanged(newSvgFile);
}

void TextItem::setSvgFile(const QString& newSvgFile)
{
    setSvgFile(QFileInfo(newSvgFile));
}

void TextItem::paint(QPainter* painter)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing, true);

    QSvgRenderer renderer(m_svg_file.absoluteFilePath());

    const auto size = renderer.defaultSize();
    parentItem()->setWidth(size.width());
    parentItem()->setHeight(size.height());
    setWidth(size.width());
    setHeight(size.height());

    renderer.render(painter, QRect(QPoint(x(), y()), size));
    painter->restore();
}

QJsonObject TextItem::toJson() const
{
    auto json = AbstractItem::toJson();

    json["item.latexSource"] = m_latex_source;
    json["item.svgFile"] = m_svg_file.absoluteFilePath();

    return json;
}

QString TextItem::getLatexSource() const
{
    return m_latex_source;
}

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

    const auto hash = QString(
        QCryptographicHash::hash(latexTemplate.toUtf8(), QCryptographicHash::Md5).toHex());

    QFileInfo svgFile(hash + ".svg");
    if (svgFile.exists()) {
        m_latex_source = newLatexSource;
        emit latexSourceChanged(newLatexSource);

        setSvgFile(svgFile);
        return;
    }

    QFile latexFile(hash + ".tex");
    if (!latexFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open: " << latexFile.fileName();
        return;
    }

    QTextStream stream(&latexFile);
    stream << latexTemplate;
    latexFile.close();

    qInfo() << QProcess::execute("latexmk", QStringList {} << "-dvi" << latexFile.fileName());
    qInfo() << QProcess::execute("dvisvgm",
        QStringList {} << hash + ".dvi"
                       << "-n"
                       << "-o" << hash + ".svg");
    qInfo() << QProcess::execute("latexmk", QStringList {} << "-C");

    setSvgFile(QFileInfo(hash + ".svg"));

    m_latex_source = newLatexSource;
    emit latexSourceChanged(newLatexSource);
}
