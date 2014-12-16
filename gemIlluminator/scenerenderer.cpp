#include "scenerenderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QDebug>

#include "abstractgem.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
,   m_initialized(false)
,   m_gl(new QOpenGLFunctions())
,   m_gemProgram(nullptr)
,   m_lightProgram(nullptr)
{
    m_gl->initializeOpenGLFunctions();
}

void SceneRenderer::initialize() {
    m_gemProgram = new QOpenGLShaderProgram(this);
    m_gemProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_gemProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_gemProgram->link()) {
        qDebug() << "Gem: Link failed";
    }

    m_gemProgram->bindAttributeLocation("a_vertex", 0);
    m_gemProgram->bindAttributeLocation("a_color", 1);
    m_gemProgram->bindAttributeLocation("a_normal", 2);

    m_lightProgram = new QOpenGLShaderProgram(this);
    m_lightProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_lightProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_lightProgram->link()) {
        qDebug() << "Light: Link failed";
    }

    m_lightProgram->bindAttributeLocation("a_vertex", 0);
    m_lightProgram->bindAttributeLocation("a_color", 1);
    m_lightProgram->bindAttributeLocation("a_normal", 2);

    m_initialized = true;
}
LightRay *SceneRenderer::rootLightRay() const
{
    return m_rootLightRay;
}

void SceneRenderer::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
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

        if (!m_initialized) {
            initialize();
        }

        /* Paint gems */
        m_gemProgram->bind();

        m_gemProgram->enableAttributeArray(0);
        m_gemProgram->enableAttributeArray(1);
        m_gemProgram->enableAttributeArray(2);

        for (auto& geometry : m_geometries) {
            geometry->paint(m_gl, m_viewProjection, *m_gemProgram);
        }

        m_gemProgram->disableAttributeArray(0);
        m_gemProgram->disableAttributeArray(1);
        m_gemProgram->disableAttributeArray(2);

        m_gemProgram->release();

        /* Paint light */
        m_lightProgram->bind();

        m_lightProgram->enableAttributeArray(0);
        m_lightProgram->enableAttributeArray(1);
        m_lightProgram->enableAttributeArray(2);

        // light paint

        m_lightProgram->disableAttributeArray(0);
        m_lightProgram->disableAttributeArray(1);
        m_lightProgram->disableAttributeArray(2);

        m_lightProgram->release();

        // Reset OpenGL state for qml
        // According to https://qt.gitorious.org/qt/qtdeclarative/source/fa0eea53f73c9b03b259f075e4cd5b83bfefccd3:src/quick/items/qquickwindow.cpp
        m_gl->glDisable(GL_DEPTH_TEST);
        m_gl->glClearColor(0, 0, 0, 0);
        m_gl->glDepthMask(GL_TRUE);
        m_gl->glDepthFunc(GL_LESS);
    }
}

void SceneRenderer::setViewport(QSize viewport)
{
    m_viewport = viewport;
}

void SceneRenderer::setGeometries(QList<AbstractGem*> geometries)
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
