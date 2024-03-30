#include "latexprocess.h"

#include "svg_config.h"

LaTeXProcess::LaTeXProcess(const QFileInfo& latex_file, QObject* parent)
    : QProcess(parent)
    , m_latex_file(latex_file)
{
    setWorkingDirectory(SVGConfig::getInstance().svgDir().absolutePath());
    setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
    setProgram(QStandardPaths::findExecutable("latex"));
    setArguments(QStringList {} << "-output-format=dvi"
                                << "-interaction=batchmode" << m_latex_file.absoluteFilePath());

    connect(this, &QProcess::finished, this, &LaTeXProcess::onFinished);
}

void LaTeXProcess::cleanup() const
{
    const QList<QString> file_appendices { ".aux", ".dvi", ".log" };
    const auto svg_dir = SVGConfig::getInstance().svgDir();

    for (const auto& appendix : file_appendices) {
        QFile::remove(svg_dir.absoluteFilePath(m_latex_file.baseName() + appendix));
    }
}

// jscpd:ignore-start
void LaTeXProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0 || exitStatus != QProcess::ExitStatus::NormalExit) {
        emit processFailed();
        return;
    }
    emit processFinished(
        QFileInfo(SVGConfig::getInstance().svgDir().absoluteFilePath(m_latex_file.baseName() + ".dvi")));
}
// jscpd:ignore-start
