#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "audioplayer.h"
#include "audioplayercontroller.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setOrganizationName("NeVr0t1k Corp.");
    app.setOrganizationDomain("nevr0t1k.com");
    app.setApplicationName("AudioPlayer");
    app.setApplicationVersion("1.1.0");

    QQmlApplicationEngine engine;
    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    AudioPlayer player;
    qmlRegisterType<AudioPlayer>("Player", 1, 0, "AudioPlayer");
    qmlRegisterType<AudioPlayerController>("Player", 1, 0, "AudioPlayerController");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);
    engine.load(url);

    return app.exec();
}
