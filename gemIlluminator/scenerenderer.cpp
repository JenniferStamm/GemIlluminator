#include "scenerenderer.h"

#include <QOpenGLFunctions>

#include "abstractgeometry.h"

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
    m_gl->glDisable(GL_CULL_FACE);

    m_gl->glEnable(GL_DEPTH_TEST);
    m_gl->glDepthFunc(GL_LEQUAL);
    m_gl->glDepthMask(GL_TRUE);

    for (auto& geometry : m_geometries) {
        geometry->paint(m_gl);
    }
}

void SceneRenderer::setViewport(QSize viewport)
{
    m_viewport = viewport;
}

void SceneRenderer::setGeometries(QList<AbstractGeometry*> geometries)
{
    m_geometries = geometries;
}

bool SceneRenderer::isActive()
{
    return m_active;
}

void SceneRenderer::setActive(bool active)
{
    m_active = active;
}
