#include "scenerenderer.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
{
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
