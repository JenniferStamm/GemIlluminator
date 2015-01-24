#include "abstractgemrenderer.h"

#include <QOpenGLShaderProgram>
#include <QVector3D>

AbstractGemRenderer::AbstractGemRenderer(QObject *parent) :
    QObject(parent)
  , m_initialRotation(new QVector3D)
  , m_position(new QVector3D())
  , m_rotation(new QVector3D())
  , m_scale(1.f)
  , m_color(new QVector3D())
{
}

AbstractGemRenderer::~AbstractGemRenderer()
{
    delete m_initialRotation;
    delete m_position;
    delete m_rotation;
    delete m_color;
}

const QVector3D &AbstractGemRenderer::initialRotation() const
{
    return *m_initialRotation;
}

void AbstractGemRenderer::setInitialRotation(const QVector3D &initialRotation)
{
    if (initialRotation == *m_initialRotation) {
       return;
    }
    *m_initialRotation = initialRotation;
}

const QVector3D &AbstractGemRenderer::position() const
{
    return *m_position;
}

void AbstractGemRenderer::setPosition(const QVector3D &position)
{
    if (position == *m_position) {
       return;
    }
    *m_position = position;
}

const QVector3D &AbstractGemRenderer::rotation() const
{
    return *m_rotation;
}

void AbstractGemRenderer::setRotation(const QVector3D &rotation)
{
    if (rotation == *m_rotation) {
       return;
    }
    *m_rotation = rotation;
}

qreal AbstractGemRenderer::scale() const
{
    return m_scale;
}

void AbstractGemRenderer::setScale(qreal scale)
{
    if (scale == m_scale) {
       return;
    }
    m_scale = scale;
}

const QVector3D &AbstractGemRenderer::color() const
{
    return *m_color;
}

void AbstractGemRenderer::setColor(const QVector3D &color)
{
    *m_color = color;
}

