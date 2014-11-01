#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "cube.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Define a C++ object for use in qml
    qmlRegisterType<Cube>("GPCube", 1, 0, "Cube");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    return app.exec();
}
