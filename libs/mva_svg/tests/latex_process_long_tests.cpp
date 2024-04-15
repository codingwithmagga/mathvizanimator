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

#include "latex_process.h"
#include "svg_config.h"
#include "svg_test_helper_functions.h"

class TestLaTeXProcess : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();

    void renderHelloWorld();
    void renderError();
};

void TestLaTeXProcess::initTestCase() { SVGConfig::getInstance().setSVGDir(QDir::current()); }

void TestLaTeXProcess::renderHelloWorld()
{
    LaTeXProcess latex_process(SVGTestHelperFunctions::createLocalFile("://test_data/latex_hello_world.tex"));
    if (QFile::exists("latex_hello_world_copy.dvi")) {
        SVGTestHelperFunctions::removeFile("latex_hello_world_copy.dvi");
    }

    QSignalSpy spyFinished(&latex_process, &LaTeXProcess::processFinished);
    QSignalSpy spyError(&latex_process, &LaTeXProcess::processFailed);
    connect(&latex_process, &LaTeXProcess::processFinished, this, [&](const QFileInfo& dvi_file) {
        QVERIFY(QFile::exists(dvi_file.absoluteFilePath()));

        latex_process.cleanup();
        QVERIFY(!QFile::exists(dvi_file.absoluteFilePath()));
        QVERIFY(!QFile::exists(QFileInfo(dvi_file.baseName() + ".log").absoluteFilePath()));
        QVERIFY(!QFile::exists(QFileInfo(dvi_file.baseName() + ".aux").absoluteFilePath()));
    });

    latex_process.start();
    QVERIFY(spyFinished.wait(20000));
    QCOMPARE(spyError.count(), 0);
}

void TestLaTeXProcess::renderError()
{
    const auto latex_file = SVGTestHelperFunctions::createLocalFile("://test_data/latex_error.tex");
    LaTeXProcess latex_process(latex_file);

    QSignalSpy spyFinished(&latex_process, &LaTeXProcess::processFinished);
    QSignalSpy spyError(&latex_process, &LaTeXProcess::processFailed);

    connect(&latex_process, &LaTeXProcess::processFailed, this, [&](const QFileInfo& failed_latex_file) {
        latex_process.cleanup();
        QCOMPARE(latex_file.absoluteFilePath(), failed_latex_file.absoluteFilePath());
    });

    latex_process.start();
    QVERIFY(spyError.wait(10000));
    QCOMPARE(spyFinished.count(), 0);
}

QTEST_MAIN(TestLaTeXProcess)
#include "latex_process_long_tests.moc"
