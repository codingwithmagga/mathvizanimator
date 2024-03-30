#include "svgcreator.h"

#include <QCryptographicHash>
#include <QProcess>
#include <QStandardPaths>

#include "dvisvgmprocess.h"
#include "latexprocess.h"
#include "svg_config.h"

SVGCreator::SVGCreator(QObject* parent)
    : QObject { parent }
{
}

void SVGCreator::renderLaTeX(const QString& latex)
{
    const auto hash = SVGConfig::hash(latex);

    QFileInfo svgFile(SVGConfig::getInstance().svgDir().absoluteFilePath(hash + ".svg"));
    if (svgFile.exists()) {
        emit latexRenderingFinished(svgFile);
        return;
    }

    const auto latex_process = QSharedPointer<LaTeXProcess>(new LaTeXProcess(prepareLaTeXFile(latex)));
    m_latex_process_map.insert(hash, latex_process);

    connect(latex_process.data(), &LaTeXProcess::processFinished, this,
        [&](const QFileInfo& dvi_file) { latexProcessFinished(dvi_file); });
    connect(latex_process.data(), &LaTeXProcess::errorOccurred, this, &SVGCreator::latexProcessFailed);
    latex_process->start();
}

void SVGCreator::latexProcessFinished(const QFileInfo& dvi_file)
{
    const auto hash = dvi_file.baseName();

    const auto dvisvgm_process = QSharedPointer<DvisvgmProcess>(new DvisvgmProcess(dvi_file));
    m_dvisvgm_process_map.insert(hash, dvisvgm_process);

    connect(dvisvgm_process.data(), &DvisvgmProcess::processFinished, this,
        [&](const QFileInfo& dvi_file) { dvisvgmProcessFinished(dvi_file); });

    dvisvgm_process->start();
}

void SVGCreator::dvisvgmProcessFinished(const QFileInfo& svg_file)
{
    const auto hash = svg_file.baseName();

    m_dvisvgm_process_map.take(hash);
    const auto latex_process = m_latex_process_map.take(hash);
    latex_process->cleanup();

    emit latexRenderingFinished(svg_file);
}

void SVGCreator::latexProcessFailed(QProcess::ProcessError error)
{
    qDebug() << "LaTeX process failed with error:" << error;
    const auto latex_process = qobject_cast<LaTeXProcess*>(sender());
    latex_process->cleanup();
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
