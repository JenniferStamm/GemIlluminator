#include "scenerenderer.h"

#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QSize>
#include <QString>
#include <QDebug>

#include "abstractgem.h"
#include "lightray.h"
#include "screenalignedquad.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_initialized(false)
  , m_viewport(new QSize())
  , m_gl(new QOpenGLFunctions())
  , m_projectionInverted(new QMatrix4x4())
  , m_view(new QMatrix4x4())
  , m_viewProjection(new QMatrix4x4())
  , m_gemProgram(nullptr)
  , m_lightProgram(nullptr)
  , m_envmapProgram(nullptr)
  , m_quad(nullptr)
{
    m_gl->initializeOpenGLFunctions();
}

SceneRenderer::~SceneRenderer()
{
    delete m_gl;
    delete m_projectionInverted;
    delete m_quad;
    delete m_view;
    delete m_viewport;
    delete m_viewProjection;
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

    initializeEnvmap();

    m_initialized = true;
}

void SceneRenderer::initializeEnvmap()
{
    // Initialize squad
    m_quad = new ScreenAlignedQuad(*m_gl);

    // Initialize Cube Map
    QMap<GLenum, QImage> images;
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_X] = QImage(":/data/env_cube_px.png");
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] = QImage(":/data/env_cube_nx.png");
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] = QImage(":/data/env_cube_py.png");
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] = QImage(":/data/env_cube_ny.png");
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] = QImage(":/data/env_cube_pz.png");
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] = QImage(":/data/env_cube_nz.png");

    m_gl->glGenTextures(1, &m_envmap);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_envmap);

    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_envmap);

    QList<GLenum> faces = QList<GLenum>()
            << GL_TEXTURE_CUBE_MAP_POSITIVE_X << GL_TEXTURE_CUBE_MAP_NEGATIVE_X
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Y << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Z << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    foreach(GLenum face, faces) {
            const QImage &image(images[face]);
            m_gl->glTexImage2D(face, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }

    m_envmapProgram = new QOpenGLShaderProgram();
    m_envmapProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/envmap.vert");
    m_envmapProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/envmap.frag");

    if (!m_envmapProgram->link()) {
        qDebug() << "Light: Link failed";
    }

    m_envmapProgram->bindAttributeLocation("a_vertex", 0);
}

void SceneRenderer::paint()
{
    if (m_active) {
        m_gl->glClearColor(0.9f, 1.f, 1.f, 1.f);
        m_gl->glClear(GL_COLOR_BUFFER_BIT);
        m_gl->glDisable(GL_CULL_FACE);

        m_gl->glEnable(GL_DEPTH_TEST);
        m_gl->glDepthFunc(GL_LEQUAL);
        m_gl->glDepthMask(GL_FALSE);

        if (!m_initialized) {
            initialize();
        }

        paintEnvmap();

        /* Paint gems */
        m_gemProgram->bind();

        m_gemProgram->enableAttributeArray(0);
        m_gemProgram->enableAttributeArray(1);
        m_gemProgram->enableAttributeArray(2);

        for (auto& geometry : m_geometries) {
            geometry->paint(*m_gl, *m_viewProjection, *m_gemProgram);
        }

        m_gemProgram->disableAttributeArray(0);
        m_gemProgram->disableAttributeArray(1);
        m_gemProgram->disableAttributeArray(2);

        m_gemProgram->release();

        /* Paint lightrays */
        m_rootLightRay->paint(*m_gl);

        // Reset OpenGL state for qml
        // According to https://qt.gitorious.org/qt/qtdeclarative/source/fa0eea53f73c9b03b259f075e4cd5b83bfefccd3:src/quick/items/qquickwindow.cpp
        m_gl->glDisable(GL_DEPTH_TEST);
        m_gl->glClearColor(0, 0, 0, 0);
        m_gl->glDepthMask(GL_TRUE);
        m_gl->glDepthFunc(GL_LESS);
    }
}

void SceneRenderer::paintEnvmap()
{
    m_envmapProgram->bind();

    m_envmapProgram->setUniformValue("view", *m_view);
    m_envmapProgram->setUniformValue("projectionInverse", *m_projectionInverted);

    m_envmapProgram->setUniformValue("cubemap", 0);

    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glEnable(GL_TEXTURE_CUBE_MAP);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_envmap);

    m_envmapProgram->bind();
    m_quad->draw(*m_gl);
    m_envmapProgram->release();

    m_gl->glDepthMask(GL_TRUE);
    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_gl->glDisable(GL_TEXTURE_CUBE_MAP);
}

/**
 * @brief SceneRenderer::setViewport Only set within sync()
 * @param viewport
 */
void SceneRenderer::setViewport(const QSize &viewport)
{
    if (*m_viewport == viewport) {
        return;
    }
    *m_viewport = viewport;
    m_gl->glViewport(0, 0, viewport.width(), viewport.height());
}

void SceneRenderer::setGeometries(QList<AbstractGem*> geometries)
{
    m_geometries = geometries;
}

void SceneRenderer::setProjectionInverted(const QMatrix4x4 &projectionInverted)
{
    *m_projectionInverted = projectionInverted;
}

void SceneRenderer::setView(const QMatrix4x4 &view)
{
    *m_view = view;
}

void SceneRenderer::setViewProjection(const QMatrix4x4 &viewProjection)
{
    *m_viewProjection = viewProjection;
}



bool SceneRenderer::isActive() const
{
    return m_active;
}

void SceneRenderer::setActive(bool active)
{
    m_active = active;
}


LightRay *SceneRenderer::rootLightRay() const
{
    return m_rootLightRay;
}

void SceneRenderer::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
}
