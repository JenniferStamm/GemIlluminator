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

void AbstractGeometry::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection)
{
    if (m_renderer) {
        m_renderer->paint(gl, viewProjection);
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
QVector3D AbstractGeometry::initialRotation() const
{
    return m_initialRotation;
}

void AbstractGeometry::setInitialRotation(const QVector3D &initialRotation)
{
    if (initialRotation == m_initialRotation) {
       return;
    }

    m_initialRotation = initialRotation;
    emit initialRotationChanged();
}

