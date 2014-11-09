#include <QtQuick/QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>
//<QQuickItem> is required for qmlRegisterType<T>()
#include <QQuickItem>

#include "cube.h"
#include "scene.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Define a C++ object for use in qml
    qmlRegisterType<Scene>("OpenGLScene", 1, 0, "Scene");
    qmlRegisterType<Cube>("OpenGLScene", 1, 0, "Cube");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
