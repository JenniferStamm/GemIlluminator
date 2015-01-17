#include <QApplication>
#include <QQmlApplicationEngine>

#include "abstractgem.h"
#include "scene.h"
#include "cubegem.h"
#include "tetrahedrongem.h"
#include "player.h"
#include "lightray.h"
#include "camera.h"
#include "navigation.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Define a C++ object for use in qml
    qmlRegisterType<AbstractGem>();
    qmlRegisterType<Scene>("GemIlluminator", 1, 0, "Scene");
    qmlRegisterType<CubeGem>("GemIlluminator", 1, 0, "CubeGem");
    qmlRegisterType<TetrahedronGem>("GemIlluminator", 1, 0, "TetrahedronGem");
    qmlRegisterType<Player>("GemIlluminator", 1, 0, "Player");
    qmlRegisterType<LightRay>("GemIlluminator", 1, 0, "LightRay");
    qmlRegisterType<Camera>("GemIlluminator", 1, 0, "Camera");
    qmlRegisterType<Navigation>("GemIlluminator", 1, 0, "Navigation");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
