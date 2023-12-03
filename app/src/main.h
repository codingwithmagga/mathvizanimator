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

#ifndef APP_SRC_MAIN_H
#define APP_SRC_MAIN_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mainlogic.h"

class SetupMain {
 public:
  struct SetupObjects {
    QSharedPointer<MainLogic> mainlogic;
    QSharedPointer<QQmlApplicationEngine> engine;
  };

  SetupMain() = delete;

  static SetupObjects setupApp(
      const QSharedPointer<QGuiApplication> gui_app = Q_NULLPTR) {
    SetupObjects setup_objects;

    qSetMessagePattern(
        "%{time dd.MM.yyyy hh:mm:ss.zzz} | "
        "%{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WRN%{endif}%{"
        "if-"
        "critical}CRT%{endif}%{if-fatal}FTL%{endif} | %{category} | "
        "%{message}");

    setup_objects.mainlogic = QSharedPointer<MainLogic>(new MainLogic);
    setup_objects.engine =
        QSharedPointer<QQmlApplicationEngine>(new QQmlApplicationEngine);

    // TODO pointer to shared pointer
    setup_objects.mainlogic->initEngine(setup_objects.engine.data());

    const QUrl url("qrc:/qt/qml/cwa/mva/gui/qml/MainWindow.qml");

    if (gui_app) {
      QCoreApplication::setApplicationName("mathvizanimator");
      QCoreApplication::setOrganizationName("codingwithmagga");
      QObject::connect(
          setup_objects.engine.data(),
          &QQmlApplicationEngine::objectCreationFailed, gui_app.data(),
          []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    }

    setup_objects.engine->load(url);

    setup_objects.mainlogic->connectEngine();

    return setup_objects;
  }
};

#endif  // MAIN_H
