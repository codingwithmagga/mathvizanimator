#ifndef DVISVGM_PROCESS_H
#define DVISVGM_PROCESS_H

#include <QFileInfo>
#include <QProcess>

class DvisvgmProcess : public QProcess {

    Q_OBJECT

  public:
    DvisvgmProcess(const QFileInfo& dvi_file, QObject* parent = nullptr);

    QFileInfo dviFile() const { return m_dvi_file; }

  signals:
    void processFailed();
    void processFinished(const QFileInfo& svg_file);

  private:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    QFileInfo m_dvi_file;
};

#endif // DVISVGM_PROCESS_H
