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

#include <QSignalSpy>
#include <QStandardPaths>
#include <QTest>

#include "latexprocess.h"
#include "svg_config.h"

class TestLaTeXProcess : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();

    void renderHelloWorld();
    void renderError();

  private:
    QFileInfo createLocalFile(const QString& resource_file_path);
};

void TestLaTeXProcess::initTestCase() { SVGConfig::getInstance().setSVGDir(QDir::current()); }

void TestLaTeXProcess::renderHelloWorld()
{
    LaTeXProcess latex_process(createLocalFile("://test_data/latex_hello_world.tex"));

    QSignalSpy spyFinished(&latex_process, &LaTeXProcess::processFinished);
    QSignalSpy spyError(&latex_process, &LaTeXProcess::processFailed);
    connect(&latex_process, &LaTeXProcess::processFinished, this, [&](const QFileInfo& dvi_file) {
        QVERIFY(QFile::exists(dvi_file.absoluteFilePath()));

        latex_process.cleanup();
        QCOMPARE(SVGConfig::getInstance().svgDir().entryInfoList(QDir::NoDotAndDotDot).length(), 0);
    });

    latex_process.start();
    QVERIFY(spyFinished.wait(10000));
    QCOMPARE(spyError.count(), 0);
}

void TestLaTeXProcess::renderError()
{
    LaTeXProcess latex_process(createLocalFile("://test_data/latex_error.tex"));

    QSignalSpy spyFinished(&latex_process, &LaTeXProcess::processFinished);
    QSignalSpy spyError(&latex_process, &LaTeXProcess::processFailed);

    connect(&latex_process, &LaTeXProcess::processFailed, this, [&]() { latex_process.cleanup(); });

    latex_process.start();
    QVERIFY(spyError.wait(10000));
    QCOMPARE(spyFinished.count(), 0);
}

QFileInfo TestLaTeXProcess::createLocalFile(const QString& resource_file_path)
{
    QFile resource_file(resource_file_path);
    const QString copy_file_name
        = SVGConfig::getInstance().svgDir().absoluteFilePath(QFileInfo(resource_file.fileName()).baseName())
        + "_copy.tex";

    if (QFile::exists(copy_file_name)) {
        QFile::remove(copy_file_name);
    }
    resource_file.copy(copy_file_name);

    return QFileInfo(copy_file_name);
}

QTEST_MAIN(TestLaTeXProcess)
#include "latexprocess_long_tests.moc"
