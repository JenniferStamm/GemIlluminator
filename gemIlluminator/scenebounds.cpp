#include "scenebounds.h"

#include <QVector>
#include <QVector3D>

#include "triangle.h"

SceneBounds::SceneBounds(QObject *parent) :
    AbstractGem(parent)
{
    float value = 25.f;
    m_radius = sqrt(value * value * 3.f);
    m_scale = 1.f;

    //front
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value,  value),
                            QVector3D( value,  value,  value),
                            QVector3D( value, -value,  value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value,  value),
                            QVector3D( value, -value,  value),
                            QVector3D(-value, -value,  value),
                            this));
    //back
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value, -value),
                            QVector3D(-value, -value, -value),
                            QVector3D( value, -value, -value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value, -value),
                            QVector3D( value, -value, -value),
                            QVector3D( value,  value, -value),
                            this));
    //left
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value, -value),
                            QVector3D(-value,  value,  value),
                            QVector3D(-value, -value,  value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value, -value),
                            QVector3D(-value, -value,  value),
                            QVector3D(-value, -value, -value),
                            this));
    //right
    m_triangles->append(new Triangle(
                            QVector3D( value,  value, -value),
                            QVector3D( value, -value, -value),
                            QVector3D( value, -value,  value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D( value,  value, -value),
                            QVector3D( value, -value,  value),
                            QVector3D( value,  value,  value),
                            this));
    //top
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value,  value),
                            QVector3D(-value,  value, -value),
                            QVector3D( value,  value, -value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-value,  value, -value),
                            QVector3D( value,  value, -value),
                            QVector3D( value,  value,  value),
                            this));
    //bottom
    m_triangles->append(new Triangle(
                            QVector3D(-value, -value,  value),
                            QVector3D( value, -value,  value),
                            QVector3D( value, -value, -value),
                            this));
    m_triangles->append(new Triangle(
                            QVector3D(-value, -value,  value),
                            QVector3D( value, -value, -value),
                            QVector3D(-value, -value, -value),
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
