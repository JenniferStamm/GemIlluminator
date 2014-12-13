#include "abstractgeometry.h"

#include "abstractgeometryrenderer.h"

AbstractGeometry::AbstractGeometry(QObject *parent) :
    QObject(parent)
  , m_renderer(0)
{
}

AbstractGeometry::~AbstractGeometry()
{
    delete m_renderer;
}

void AbstractGeometry::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program)
{
    if (m_renderer) {
        m_renderer->paint(gl, viewProjection, program);
    }
}

QVector3D AbstractGeometry::position()
{
    return m_position;
}

void AbstractGeometry::setPosition(QVector3D position)
{
    if (position == m_position) {
       return;
    }

    m_position = position;
    emit positionChanged();
}

QVector3D AbstractGeometry::rotation()
{
    return m_rotation;
}

void AbstractGeometry::setRotation(QVector3D rotation)
{
    if (rotation == m_rotation) {
       return;
    }

    m_rotation = rotation;
    emit rotationChanged();
}
