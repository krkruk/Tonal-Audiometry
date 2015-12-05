#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include "appengine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    auto rootObjs = engine.rootObjects();
    AppEngine appEngine(rootObjs[0]);

    Q_UNUSED(appEngine)

    return app.exec();
}
