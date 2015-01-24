#include "scenebounds.h"

#include <QVector>
#include <QVector3D>

#include "triangle.h"

SceneBounds::SceneBounds(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(15.f * 15.f * 3.f);
    m_scale = 1.f;

    //front
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f,  15.f),
                            QVector3D( 15.f,  15.f,  15.f),
                            QVector3D( 15.f, -15.f,  15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f,  15.f),
                            QVector3D( 15.f, -15.f,  15.f),
                            QVector3D(-15.f, -15.f,  15.f),
                            this));
    //back
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f, -15.f),
                            QVector3D(-15.f, -15.f, -15.f),
                            QVector3D( 15.f, -15.f, -15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f, -15.f),
                            QVector3D( 15.f, -15.f, -15.f),
                            QVector3D(+15.f, +15.f, -15.f),
                            this));
    //left
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f,  -15.f),
                            QVector3D(-15.f,  15.f,   15.f),
                            QVector3D(-15.f, -15.f,   15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f, -15.f),
                            QVector3D(-15.f, -15.f,  15.f),
                            QVector3D(-15.f, -15.f, -15.f),
                            this));
    //right
    m_triangles->append(new Triangle(
                            QVector3D( 15.f,  15.f,  -15.f),
                            QVector3D( 15.f, -15.f,  -15.f),
                            QVector3D( 15.f, -15.f,   15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D( 15.f,  15.f, -15.f),
                            QVector3D( 15.f, -15.f,  15.f),
                            QVector3D( 15.f,  15.f,  15.f),
                            this));
    //top
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f,  15.f),
                            QVector3D(-15.f,  15.f, -15.f),
                            QVector3D( 15.f,  15.f, -15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-15.f,  15.f, -15.f),
                            QVector3D( 15.f,  15.f, -15.f),
                            QVector3D( 15.f,  15.f,  15.f),
                            this));
    //bottom
    m_triangles->append(new Triangle(
                            QVector3D(-15.f, -15.f,  15.f),
                            QVector3D( 15.f, -15.f,  15.f),
                            QVector3D( 15.f, -15.f, -15.f),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-15.f, -15.f,  15.f),
                            QVector3D( 15.f, -15.f, -15.f),
                            QVector3D(-15.f, -15.f, -15.f),
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

void SceneBounds::update(int timeDifference)
{
}

void SceneBounds::paint(QOpenGLFunctions &/*gl*/, const QMatrix4x4 &/*viewprojection*/, QOpenGLShaderProgram &/*program*/)
{
}

void SceneBounds::setInitialRotation(const QVector3D &/*initialRotation*/)
{
}

void SceneBounds::setPosition(const QVector3D &/*position*/)
{
}

void SceneBounds::setRotation(const QVector3D &/*rotation*/)
{
}
