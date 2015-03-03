#include "scenebounds.h"

#include <QtGlobal>
#include <QList>
#include <QVector>
#include <QVector3D>

#include "config.h"
#include "gemdata.h"
#include "lightray.h"
#include "triangle.h"

SceneBounds::SceneBounds(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(3.f);
    float axisRange = Config::instance()->axisRange();
    setScale(qMin(axisRange * 1.1f, axisRange + 5.f));

    //front
    QList<Triangle *> triangles;
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f)));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f)));
    //back
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f)));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f)));
    //left
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f)));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f)));
    //right
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f)));
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f)));
    //top
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f)));
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f, -1.0f)));
    //bottom
    triangles.append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f)));
    triangles.append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f)));
    m_data->setTriangles(triangles);
}

SceneBounds::~SceneBounds()
{
}

void SceneBounds::setPosition(const QVector3D &/*position*/)
{
}

void SceneBounds::setRotation(const QQuaternion &/*rotation*/)
{
}

QList<LightRay *> SceneBounds::processRayIntersection(const LightRay &ray, Scene *scene)
{
    emit gameLost();
    QList<LightRay *> result;
    result.append(new LightRay());
    return result;
}
