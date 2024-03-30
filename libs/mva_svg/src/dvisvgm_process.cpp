#include "dvisvgm_process.h"

#include "svg_config.h"

DvisvgmProcess::DvisvgmProcess(const QFileInfo& dvi_file, QObject* parent)
    : QProcess(parent)
    , m_dvi_file(dvi_file)
{
    setWorkingDirectory(SVGConfig::getInstance().svgDir().absolutePath());
    setProcessChannelMode(QProcess::ProcessChannelMode::MergedChannels);
    setProgram(QStandardPaths::findExecutable("dvisvgm"));
    setArguments(QStringList {} << m_dvi_file.absoluteFilePath() << "-n"
                                << "-o" << m_dvi_file.baseName() + ".svg");

    connect(this, &QProcess::finished, this, &DvisvgmProcess::onFinished);
}

// jscpd:ignore-start
void DvisvgmProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0 || exitStatus != QProcess::ExitStatus::NormalExit) {
        emit processFailed();
        return;
    }
    emit processFinished(QFileInfo(SVGConfig::getInstance().svgDir().absoluteFilePath(m_dvi_file.baseName() + ".svg")));
}
// jscpd:ignore-end
