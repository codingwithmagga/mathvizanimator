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

#include "dvisvgm_process.h"
#include "svg_config.h"
#include "svg_test_helper_functions.h"

class TestDvisvgmProcess : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();

    void renderHelloWorld();
    void renderError();
};

void TestDvisvgmProcess::initTestCase() { SVGConfig::getInstance().setSVGDir(QDir::current()); }

void TestDvisvgmProcess::renderHelloWorld()
{
    const auto local_dvi_file = SVGTestHelperFunctions::createLocalFile("://test_data/latex_hello_world.dvi");
    DvisvgmProcess dvi_process(local_dvi_file);

    QSignalSpy spyFinished(&dvi_process, &DvisvgmProcess::processFinished);
    QSignalSpy spyError(&dvi_process, &DvisvgmProcess::processFailed);
    connect(&dvi_process, &DvisvgmProcess::processFinished, this, [&](const QFileInfo& svg_file) {
        QVERIFY(QFile::exists(svg_file.absoluteFilePath()));

        const QString validation_svg = SVGTestHelperFunctions::readData("://validation_data/hello_world.svg");
        QCOMPARE(SVGTestHelperFunctions::readData(svg_file.absoluteFilePath()), validation_svg);
    });

    dvi_process.start();
    QVERIFY(spyFinished.wait(10000));
    QCOMPARE(spyError.count(), 0);
}

void TestDvisvgmProcess::renderError()
{
    DvisvgmProcess dvi_process(QFileInfo("://test_data/latex_hello_world.dvi"));

    QSignalSpy spyFinished(&dvi_process, &DvisvgmProcess::processFinished);
    QSignalSpy spyError(&dvi_process, &DvisvgmProcess::processFailed);

    dvi_process.start();
    QVERIFY(spyError.wait(10000));
    QCOMPARE(spyFinished.count(), 0);
}

QTEST_MAIN(TestDvisvgmProcess)
#include "dvisvgm_process_long_tests.moc"
