#include "svgcreator.h"

#include <QCryptographicHash>
#include <QProcess>
#include <QStandardPaths>

#include "latexprocess.h"
#include "svg_config.h"

SVGCreator::SVGCreator(QObject* parent)
    : QObject { parent }
{
    m_dvisvgm_path = QStandardPaths::findExecutable("dvisvgm");
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

    QProcess dvisvgm_process;
    dvisvgm_process.setWorkingDirectory(SVGConfig::getInstance().svgDir().absolutePath());
    dvisvgm_process.start(m_dvisvgm_path,
        QStringList {} << dvi_file.absoluteFilePath() << "-n"
                       << "-o" << hash + ".svg");

    const auto latex_process = m_latex_process_map.take(hash);
    if (!dvisvgm_process.waitForFinished()) {
        qDebug() << "Make failed:" << dvisvgm_process.errorString();
        latex_process->cleanup();
        return;
    }
    latex_process->cleanup();

    const QFileInfo svgFile(SVGConfig::getInstance().svgDir().absoluteFilePath(hash + ".svg"));
    emit latexRenderingFinished(svgFile);
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
