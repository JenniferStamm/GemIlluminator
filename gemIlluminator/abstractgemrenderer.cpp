#include "abstractgemrenderer.h"

#include <QOpenGLShaderProgram>
#include <QQuaternion>
#include <QVector3D>

AbstractGemRenderer::AbstractGemRenderer(QObject *parent) :
    QObject(parent)
  , m_color(new QVector3D())
  , m_model(new QMatrix4x4())
{
}

AbstractGemRenderer::~AbstractGemRenderer()
{
    delete m_color;
    delete m_model;
}

const QVector3D &AbstractGemRenderer::color() const
{
    return *m_color;
}

void AbstractGemRenderer::setColor(const QVector3D &color)
{
    *m_color = color;
}

const QMatrix4x4 &AbstractGemRenderer::model() const
{
    return *m_model;
}

void AbstractGemRenderer::setModel(const QMatrix4x4 &model)
{
    *m_model = model;
}
