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

#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>
#include <QtQml/QQmlExtensionPlugin>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QSurfaceFormat format = QSurfaceFormat::defaultFormat();
  format.setSamples(4);
  QSurfaceFormat::setDefaultFormat(format);

  qSetMessagePattern(
      "%{time dd.MM.yyyy hh:mm:ss.zzz} | "
      "%{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WRN%{endif}%{if-"
      "critical}CRT%{endif}%{if-fatal}FTL%{endif} | %{category} | %{message}");

  QCoreApplication::setApplicationName("mathvizanimator");
  QCoreApplication::setOrganizationName("codingwithmagga");

  QGuiApplication app(argc, argv);

  MainWindowHandler main_window;
  QQmlApplicationEngine engine;

  main_window.initEngine(&engine);

  const QUrl url("qrc:/qt/qml/cwa/mva/app/qml/Main.qml");
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.load(url);

  main_window.init();

  return app.exec();
}
