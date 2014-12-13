#include "scenerenderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QDebug>

#include "abstractgeometry.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
,   m_gl(new QOpenGLFunctions())
,   m_program(nullptr)
{
    m_gl->initializeOpenGLFunctions();
}

void SceneRenderer::initialize() {
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_program->link()) {
        qDebug() << "Link failed";
    }

    m_program->bindAttributeLocation("a_vertex", 0);
    m_program->bindAttributeLocation("a_color", 1);
    m_program->bindAttributeLocation("a_normal", 2);
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

        if (!m_program) {
            initialize();
        }
        m_program->bind();

        m_gl->glEnableVertexAttribArray(0);
        m_gl->glEnableVertexAttribArray(1);
        m_gl->glEnableVertexAttribArray(2);

        for (auto& geometry : m_geometries) {
            geometry->paint(m_gl, m_viewProjection, *m_program);
        }

        m_gl->glDisableVertexAttribArray(0);
        m_gl->glDisableVertexAttribArray(1);
        m_gl->glDisableVertexAttribArray(2);

        m_program->release();

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
