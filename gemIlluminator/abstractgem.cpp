#include "abstractgem.h"

#include "abstractgemrenderer.h"

AbstractGem::AbstractGem(QObject *parent) :
    QObject(parent)
  , m_renderer(0)
{
}

AbstractGem::~AbstractGem()
{
    delete m_renderer;
}

void AbstractGem::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program)
{
    if (m_renderer) {
        m_renderer->paint(gl, viewProjection, program);
    }
}

QVector3D AbstractGem::position()
{
    return m_position;
}

void AbstractGem::setPosition(QVector3D position)
{
    if (position == m_position) {
       return;
    }

    m_position = position;
    emit positionChanged();
}

QVector3D AbstractGem::rotation()
{
    return m_rotation;
}

void AbstractGem::setRotation(QVector3D rotation)
{
    if (rotation == m_rotation) {
       return;
    }

    m_rotation = rotation;
    emit rotationChanged();
}
