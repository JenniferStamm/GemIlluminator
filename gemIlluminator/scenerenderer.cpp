#include "scenerenderer.h"

#include <QOpenGLFunctions>

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
,   m_gl(new QOpenGLFunctions())
{
    m_gl->initializeOpenGLFunctions();
}

void SceneRenderer::paint()
{
    m_gl->glClearColor(0.9f, 1.f, 1.f, 1.f);
    m_gl->glClear(GL_COLOR_BUFFER_BIT);
}

void SceneRenderer::setViewport(QSize viewport)
{
    m_viewport = viewport;
}

bool SceneRenderer::isActive()
{
    return m_active;
}

void SceneRenderer::setActive(bool active)
{
    m_active = active;
}
