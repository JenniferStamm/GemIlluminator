#include <QApplication>
#include <QQmlApplicationEngine>

#include "abstractgeometry.h"
#include "scene.h"
#include "gem.h"
#include "player.h"
#include "lightray.h"
#include "camera.h"
#include "navigation.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define a C++ object for use in qml
    qmlRegisterType<AbstractGeometry>();
    qmlRegisterType<Scene>("GemIlluminator", 1, 0, "Scene");
    qmlRegisterType<Gem>("GemIlluminator", 1, 0, "Gem");
    qmlRegisterType<Player>("GemIlluminator", 1, 0, "Player");
    qmlRegisterType<LightRay>("GemIlluminator", 1, 0, "LightRay");
    qmlRegisterType<Camera>("GemIlluminator", 1, 0, "Camera");
    qmlRegisterType<Navigation>("GemIlluminator", 1, 0, "Navigation");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
