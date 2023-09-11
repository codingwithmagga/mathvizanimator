#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/QQmlExtensionPlugin>

#include "QDirIterator"

#include "mainwindow.h"

Q_IMPORT_QML_PLUGIN(mva_guiPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MainWindow main_window;
    engine.rootContext()->setContextProperty(QStringLiteral("main_window"), &main_window);

    auto qmlImageProvider = main_window.getImageProvider();
    engine.addImageProvider(QLatin1String("imageprovider"), qmlImageProvider);
    engine.rootContext()->setContextProperty("imageprovider", qmlImageProvider);

    const QUrl url("qrc:/qt/qml/mathvizanimator/src/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
