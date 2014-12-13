#include "abstractgemrenderer.h"

#include <QOpenGLShaderProgram>

AbstractGemRenderer::AbstractGemRenderer(QObject *parent) :
    QObject(parent)
{
}

AbstractGemRenderer::~AbstractGemRenderer()
{
}

QVector3D AbstractGemRenderer::position()
{
    return m_position;
}

void AbstractGemRenderer::setPosition(QVector3D position)
{
    if (position == m_position) {
       return;
    }

    m_position = position;
}

QVector3D AbstractGemRenderer::rotation()
{
    return m_rotation;
}

void AbstractGemRenderer::setRotation(QVector3D rotation)
{
    if (rotation == m_rotation) {
       return;
    }

    m_rotation = rotation;
}
QVector3D AbstractGemRenderer::initialRotation() const
{
    return m_initialRotation;
}

void AbstractGemRenderer::setInitialRotation(const QVector3D &initialRotation)
{
    if (initialRotation == m_initialRotation) {
       return;
    }

    m_initialRotation = initialRotation;
}

