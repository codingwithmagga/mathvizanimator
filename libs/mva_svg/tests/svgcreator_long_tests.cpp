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

#include "svg_config.h"
#include "svgcreator.h"

class TestSVGCreator : public QObject {
    Q_OBJECT

  private slots:
    void initTestCase();

    void renderHelloWorld();

  private:
    QString readData(QString file_path);
};

void TestSVGCreator::initTestCase() { SVGConfig::getInstance().setSVGDir(QDir::current()); }

void TestSVGCreator::renderHelloWorld()
{
    const QString latex_text = readData("://test_data/latex_hello_world.tex");
    QCOMPARE_NE(latex_text, QString());
    SVGCreator svg_creator;

    QFile svg_file(SVGConfig::getInstance().svgDir().absoluteFilePath("faa2b4b9c1edd4325f54f94b8387d5b6.svg"));
    if (svg_file.exists()) {
        svg_file.remove();
    }

    QSignalSpy spy(&svg_creator, &SVGCreator::latexRenderingFinished);
    connect(&svg_creator, &SVGCreator::latexRenderingFinished, this, [&](const QFileInfo& created_svg_file) {
        QVERIFY(svg_file.exists());

        const QString validation_svg = readData("://validation_data/hello_world.svg");
        QCOMPARE(readData(created_svg_file.absoluteFilePath()), validation_svg);

        QVERIFY(!QFile::exists(QFileInfo(created_svg_file.baseName() + ".dvi").absoluteFilePath()));
        QVERIFY(!QFile::exists(QFileInfo(created_svg_file.baseName() + ".log").absoluteFilePath()));
        QVERIFY(!QFile::exists(QFileInfo(created_svg_file.baseName() + ".aux").absoluteFilePath()));
    });

    svg_creator.renderLaTeX(latex_text);
    QVERIFY(spy.wait(10000));
}

QString TestSVGCreator::readData(QString file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Cannot open: " << file.fileName();
        return QString();
    }

    QString data = file.readAll();
    file.close();

    return data;
}

QTEST_MAIN(TestSVGCreator)
#include "svgcreator_long_tests.moc"
