#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "appengine.h"
#include <QList>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    AppEngine appEngine;


    engine.rootContext()->setContextProperty("appEngine", &appEngine);
    engine.addImageProvider(QLatin1String("audiogram"), new AudiogramChartWidget(&appEngine));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    auto rootObjs = engine.rootObjects();
    appEngine.setRootQmlObject(rootObjs[0]);
    appEngine.connectAll();

    return app.exec();
}
