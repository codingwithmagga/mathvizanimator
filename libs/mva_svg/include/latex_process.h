#ifndef LATEX_PROCESS_H
#define LATEX_PROCESS_H

#include <QFileInfo>
#include <QProcess>

class LaTeXProcess : public QProcess {

    Q_OBJECT

  public:
    LaTeXProcess(const QFileInfo& latex_file, QObject* parent = nullptr);

    void cleanup() const;

    QFileInfo latexFile() const { return m_latex_file; }

  signals:
    void processFailed();
    void processFinished(const QFileInfo& dvi_file);

  private:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QFileInfo m_latex_file;
};

#endif // LATEX_PROCESS_H
