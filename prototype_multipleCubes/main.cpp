/*
 * Hier nun ein Prototyp mit mehreren gerenderten Cubes.
 * Es ist ein neues Projekt, daich damit in der Bahn angefangen hatte
 * und anschließend nur noch der Rendering-Code für die Würfel gefehlt hatte.
 *
 * Ich kam leider nicht um die Verwendung eines Timers für das neuzeichnen drum herum. *
 */




#include <QApplication>
#include <QQmlApplicationEngine>
//<QQuickItem> is required for qmlRegisterType<T>()
#include <QQuickItem>

#include "cube.h"
#include "scene.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Scene>("OpenGLScene", 1, 0, "Scene");
    qmlRegisterType<Cube>("OpenGLScene", 1, 0, "Cube");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
