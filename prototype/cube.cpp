#include "cube.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QVector4D>

// Qt sounds
#include <QMediaResource>
#include <QMediaContent>
#include <QMediaPlayer>

#include "cuberenderer.h"

Cube::Cube(QObject *parent) :
    QObject(parent)
  , m_renderer(new CubeRenderer())
{
}

Cube::~Cube()
{
    delete m_renderer;
}

void Cube::paint(QOpenGLFunctions *gl)
{
    m_renderer->paint(gl);
}

qreal Cube::x()
{
    return m_position.x();
}

void Cube::setX(qreal x)
{
    if(m_position.x() == x)
        return;
    m_position.setX(x);
    emit xChanged();
}

qreal Cube::y()
{
    return m_position.y();
}

void Cube::setY(qreal y)
{
    if(m_position.y() == y)
        return;
    m_position.setY(y);
    emit yChanged();
}

qreal Cube::z()
{
    return m_position.z();
}

void Cube::setZ(qreal z)
{
    if(m_position.z() == z)
        return;
    m_position.setZ(z);
    emit zChanged();
}

qreal Cube::xRotation()
{
    return m_rotation.x();
}

void Cube::setXRotation(qreal xRotation)
{
    if(m_rotation.x() == xRotation)
        return;
    m_rotation.setX(xRotation);
    emit xRotationChanged();
}

qreal Cube::yRotation()
{
    return m_rotation.y();
}

void Cube::setYRotation(qreal yRotation)
{
    if(m_rotation.y() == yRotation)
        return;
    m_rotation.setY(yRotation);
    emit yRotationChanged();
}

qreal Cube::zRotation()
{
   return m_rotation.z();
}

void Cube::setZRotation(qreal zRotation)
{
    if(m_rotation.z() == zRotation)
        return;
    m_rotation.setZ(zRotation);
    emit zRotationChanged();
}

void Cube::synchronize()
{
    m_renderer->setPosition(m_position);
    m_renderer->setRotation(m_rotation);
}
