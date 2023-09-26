#include <QDirIterator>
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

    MainWindow main_window(&engine);
    engine.rootContext()->setContextProperty(QStringLiteral("main_window"), &main_window);

    //    QDirIterator it(":", QDirIterator::Subdirectories);
    //    while (it.hasNext()) {
    //        qDebug() << it.next();
    //    }

    const QUrl url("qrc:/qt/qml/cwa/mva/gui/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    const auto rootObject = engine.rootObjects().first();
    const auto selectArea = rootObject->findChild<QObject *>("selectArea");

    // clang-format off
    QObject::connect(selectArea,
                     SIGNAL(itemAdded(QQuickItem*)),
                     &main_window,
                     SLOT(addItem(QQuickItem*)));
    // clang-format on

    return app.exec();
}
