#ifndef SVGCREATOR_H
#define SVGCREATOR_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QProcess>

#include "dvisvgmprocess.h"
#include "latexprocess.h"

class SVGCreator : public QObject {
    Q_OBJECT
  public:
    explicit SVGCreator(QObject* parent = nullptr);

    void renderLaTeX(const QString& latex);

  signals:
    void latexRenderingFinished(const QFileInfo& svg_file);

  private slots:
    void latexProcessFinished(const QFileInfo& dvi_file);
    void dvisvgmProcessFinished(const QFileInfo& svg_file);

    void latexProcessFailed(QProcess::ProcessError error);

  private:
    QFileInfo prepareLaTeXFile(const QString& latex);

    QMap<QString, QSharedPointer<LaTeXProcess>> m_latex_process_map;
    QMap<QString, QSharedPointer<DvisvgmProcess>> m_dvisvgm_process_map;

    QString m_dvisvgm_path;
};

#endif // SVGCREATOR_H
