#include "scenerenderer.h"

#include <QOpenGLFunctions>
#include <QDebug>
#include <QString>

#include "abstractgeometry.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
,   m_gl(new QOpenGLFunctions())
{
    m_gl->initializeOpenGLFunctions();
}

void SceneRenderer::paint()
{
    if (m_active) {
        m_gl->glClearColor(0.9f, 1.f, 1.f, 1.f);
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
        m_gl->glDisable(GL_CULL_FACE);

        m_gl->glEnable(GL_DEPTH_TEST);
        m_gl->glDepthFunc(GL_LEQUAL);
        m_gl->glDepthMask(GL_TRUE);
        // Hier enabling von vertexattrib
        // Program an und ausschalten
        // hier vertexattrib pointer setzen

        m_gl->glEnableVertexAttribArray(0); // nur einmal
        m_gl->glEnableVertexAttribArray(1);
        m_gl->glEnableVertexAttribArray(2);

        m_gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
        m_gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
        m_gl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (6 * sizeof(float)));

        qDebug() << QString(reinterpret_cast<const char*>(m_gl->glGetString(GL_VENDOR)));
        qDebug() << QString(reinterpret_cast<const char*>(m_gl->glGetString(GL_RENDERER)));
        for (auto& geometry : m_geometries) {
            geometry->paint(m_gl, m_viewProjection);
        }
        // State zurücsetzen??

        m_gl->glDisableVertexAttribArray(0);  // nur einmal
        m_gl->glDisableVertexAttribArray(1);
        m_gl->glDisableVertexAttribArray(2);

        // Stimmt swapBuffer mit frame anzahl überein?
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

void SceneRenderer::setViewProjection(QMatrix4x4 viewProjection)
{
    m_viewProjection = viewProjection;
}
