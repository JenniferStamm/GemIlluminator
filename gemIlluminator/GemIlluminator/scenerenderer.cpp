#include "scenerenderer.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
{
}

void SceneRenderer::setViewport(QSize viewport)
{
    m_viewport = viewport;
}
