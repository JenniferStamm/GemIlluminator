#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#ifdef __ANDROID__
#include <QtAndroidExtras>
#endif

#include "abstractgem.h"
#include "camera.h"
#include "config.h"
#include "cubegem.h"
#include "lightray.h"
#include "navigation.h"
#include "painterqml.h"
#include "player.h"
#include "scene.h"
#include "soundmanager.h"
#include "tetrahedrongem.h"

#ifdef __ANDROID__
void keepScreenOn()
{
    QAndroidJniObject activity = QtAndroid::androidActivity();

    if (activity.isValid()) {
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callObjectMethod("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
}
#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

#ifdef __ANDROID__
    keepScreenOn();
#endif

    // Define a C++ object for use in qml
    qmlRegisterType<AbstractGem>();
    qmlRegisterType<Scene>("GemIlluminator", 1, 0, "Scene");
    qmlRegisterType<CubeGem>("GemIlluminator", 1, 0, "CubeGem");
    qmlRegisterType<TetrahedronGem>("GemIlluminator", 1, 0, "TetrahedronGem");
    qmlRegisterType<Player>("GemIlluminator", 1, 0, "Player");
    qmlRegisterType<LightRay>("GemIlluminator", 1, 0, "LightRay");
    qmlRegisterType<Camera>("GemIlluminator", 1, 0, "Camera");
    qmlRegisterType<Navigation>("GemIlluminator", 1, 0, "Navigation");
    qmlRegisterSingletonType<Config>("GemIlluminator", 1, 0, "Config", configSingletontypeProvider);
    qmlRegisterSingletonType<Soundmanager>("GemIlluminator", 1, 0, "Soundmanager", soundmanagerSingletontypeProvider);
    qmlRegisterType<PainterQML>("GemIlluminator", 1, 0, "Painter");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
