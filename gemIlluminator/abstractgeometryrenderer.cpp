#include "abstractgeometryrenderer.h"

AbstractGeometryRenderer::AbstractGeometryRenderer(QObject *parent) :
    QObject(parent)
{
}

AbstractGeometryRenderer::~AbstractGeometryRenderer()
{
}

QVector3D AbstractGeometryRenderer::position()
{
    return m_position;
}

void AbstractGeometryRenderer::setPosition(QVector3D position)
{
    if (position == m_position) {
       return;
    }

    m_position = position;
}

QVector3D AbstractGeometryRenderer::rotation()
{
    return m_rotation;
}

void AbstractGeometryRenderer::setRotation(QVector3D rotation)
{
    if (rotation == m_rotation) {
       return;
    }

    m_rotation = rotation;
}
