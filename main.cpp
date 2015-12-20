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
    engine.addImageProvider(QLatin1String("audiogram"), new AudiogramChartWidget);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    auto rootObjs = engine.rootObjects();
    appEngine.setRootQmlObject(rootObjs[0]);
    appEngine.connectAll();


//    AudiogramPlotData data;
//    data.update(AudiogramData(10000,2,2));
//    data.update(AudiogramData(150,2,2));
//    data.update(AudiogramData(256,2,2));
//    auto data2=data.getSortedData();
//    for(auto elem : data2)
//        qDebug() << elem;
    return app.exec();
}
