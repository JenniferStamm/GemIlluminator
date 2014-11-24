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

void AbstractGeometry::paint(QOpenGLFunctions *gl)
{
    if (m_renderer) {
        m_renderer->paint(gl);
    }
}

void AbstractGeometry::synchronize()
{

}

QVector3D position()
{
    return m_position;
}

void setPosition(QVector3D position)
{
    if (position == m_position) {
       return;
    }

    m_position = position;
    emit positionChanged();
}

QVector3D rotation()
{
    return m_rotation;
}

void setRotation(QVector3D rotation)
{
    if (rotation == m_rotation) {
       return;
    }

    m_rotation = rotation;
    emit rotationChanged();
}
