#include "scenebounds.h"

#include <QVector>
#include <QVector3D>

#include "gemdata.h"
#include "triangle.h"

SceneBounds::SceneBounds(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(1.0f * 1.0f * 3.f);
    setScale(25.f);

    //front
    QList<Triangle *> triangles;
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            this));
    //back
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            this));
    //left
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            this));
    //right
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            this));
    //top
    triangles.append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            this));
    //bottom
    triangles.append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            this));
    triangles.append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            this));
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
