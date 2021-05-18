#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
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

    AudioPlayerController *controller = nullptr;
    qmlRegisterType<AudioPlayer>("Player", 1, 0, "AudioPlayer");
    qmlRegisterType<AudioPlayerController>("Player", 1, 0, "AudioPlayerController");

    QQuickStyle::setStyle("Material");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);
    engine.load(url);

    controller = engine.rootObjects()[0]->findChild<AudioPlayerController *>("controller");
    QObject::connect(controller->model()->client(), &Client::serverErrorDetected, &QGuiApplication::quit);

//    if (controller.model()->client()->validateTheParameters(argc)){
//        printf("usage: %s server-name\n", argv[0]);
//        return 1;
//    }

    return app.exec();
}
