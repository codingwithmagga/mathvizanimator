#ifndef SVG_CREATOR_H
#define SVG_CREATOR_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QProcess>

#include "dvisvgm_process.h"
#include "latex_process.h"

class SVGCreator : public QObject {
    Q_OBJECT
  public:
    explicit SVGCreator(QObject* parent = nullptr);

    void svgFromLaTeX(const QString& latex);

  signals:
    void svgCreated(const QFileInfo& svg_file);
    void svgCreationFailed();

  private slots:
    void latexProcessFinished(const QFileInfo& dvi_file);
    void dvisvgmProcessFinished(const QFileInfo& svg_file);

  private:
    QFileInfo prepareLaTeXFile(const QString& latex);

    QMap<QString, QSharedPointer<LaTeXProcess>> m_latex_process_map;
    QMap<QString, QSharedPointer<DvisvgmProcess>> m_dvisvgm_process_map;
};

#endif // SVG_CREATOR_H
