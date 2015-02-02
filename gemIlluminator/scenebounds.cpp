#include "scenebounds.h"

#include <QVector>
#include <QVector3D>

#include "triangle.h"

SceneBounds::SceneBounds(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(1.0f * 1.0f * 3.f);
    m_scale = 25.f;
    calculateModelMatrix();

    //front
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            this));
    //back
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            this));
    //left
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            this));
    //right
    m_triangles->append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            this));
    //top
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f, -1.0f),
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D( 1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f,  1.0f),
                            QVector3D(-1.0f,  1.0f, -1.0f),
                            this));
    //bottom
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-1.0f, -1.0f,  1.0f),
                            QVector3D( 1.0f, -1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, -1.0f),
                            this));
}

SceneBounds::~SceneBounds()
{
}

void SceneBounds::synchronize()
{
}

void SceneBounds::cleanup()
{
}

void SceneBounds::paint(QOpenGLFunctions &/*gl*/, const QMatrix4x4 &/*viewprojection*/, QOpenGLShaderProgram &/*program*/)
{
}

void SceneBounds::setPosition(const QVector3D &/*position*/)
{
}

void SceneBounds::setRotation(const QQuaternion &/*rotation*/)
{
}
