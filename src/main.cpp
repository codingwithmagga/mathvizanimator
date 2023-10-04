#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/QQmlExtensionPlugin>
#include <iostream>

#include <QQuickItem>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time dd.MM.yyyy hh:mm:ss.zzz} | "
                       "%{if-debug}DBG%{endif}%{if-info}INF%{endif}%{if-warning}WRN%{endif}%{if-"
                       "critical}CRT%{endif}%{if-fatal}FTL%{endif} | %{category} | %{message}");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MainWindowHandler main_window(&engine);

    const QUrl url("qrc:/qt/qml/cwa/mva/app/qml/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    main_window.init();

    return app.exec();
}
