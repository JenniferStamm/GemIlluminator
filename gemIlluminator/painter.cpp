#include "painter.h"

#include <QEvent>
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMap>
#include <QMatrix4x4>
#include <QSize>

#include "camera.h"
#include "lightray.h"
#include "painterqml.h"
#include "scene.h"
#include "scenerenderer.h"
#include "screenalignedquad.h"

Painter::Painter(PainterQML *painter, QObject *parent) :
    QObject(parent)
  , m_active(false)
  , m_gl(new QOpenGLFunctions())
  , m_initialized(false)
  , m_painterQML(painter)
  , m_quad(nullptr)
  , m_shaderPrograms(new QMap<ShaderPrograms, QOpenGLShaderProgram*>())
  , m_usedViewport(new QSize())
{
    m_gl->initializeOpenGLFunctions();
}

Painter::~Painter()
{
    m_gl->glDeleteTextures(1, &m_sceneTexture);
    m_gl->glDeleteTextures(1, &m_previewSceneTexture);
    m_gl->glDeleteRenderbuffers(1, &m_sceneDepthRB);
    m_gl->glDeleteRenderbuffers(1, &m_previewSceneDepthRB);
    m_gl->glDeleteFramebuffers(1, &m_sceneFBO);
    m_gl->glDeleteFramebuffers(1, &m_previewSceneFBO);

    if (m_scene) {
        m_scene->cleanupGL(*m_gl);
    }

    delete m_gl;
    delete m_quad;
    for (auto i : *m_shaderPrograms) {
        delete i;
    }
    delete m_shaderPrograms;
    delete m_usedViewport;
}

bool Painter::isActive() const
{
    return m_active;
}

void Painter::setActive(bool active)
{
    m_active = active;
}

Scene* Painter::scene() const
{
    return m_scene;
}

void Painter::setScene(Scene *scene)
{
    m_scene = scene;
}

void Painter::paint()
{
    if (m_active) {
        if (!m_initialized) {
            initialize();
        }

        m_gl->glClearColor(0.9f, 1.f, 1.f, 1.f);
        m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_gl->glDisable(GL_CULL_FACE);

        m_gl->glEnable(GL_DEPTH_TEST);
        m_gl->glDepthFunc(GL_LEQUAL);
        m_gl->glDepthMask(GL_TRUE);

        // Render to texture
        int viewportHeight = m_scene->camera()->viewport().height();
        int viewportWidth = m_scene->camera()->viewport().width();
        int previewViewportHeight = m_scene->previewCamera()->viewport().height();
        int previewViewportWidth = m_scene->previewCamera()->viewport().width();
        float previewSize = 1.f / (static_cast<float>(viewportWidth) / previewViewportWidth);

        bool viewportChanged = false;
        if (m_usedViewport->height() != viewportHeight
                && m_usedViewport->width() != viewportWidth) {
            *m_usedViewport = m_scene->camera()->viewport();
            viewportChanged = true;
        }

        // scene
        m_gl->glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);

        m_gl->glBindTexture(GL_TEXTURE_2D, m_sceneTexture);
        if (viewportChanged) {
            m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportWidth, viewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }


        m_gl->glBindRenderbuffer(GL_RENDERBUFFER, m_sceneDepthRB);
        if (viewportChanged) {
            m_gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, viewportWidth, viewportHeight);
        }
        m_gl->glViewport(0, 0, viewportWidth, viewportHeight);

        m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderScene(*m_scene->camera());

        // preview scene
        m_gl->glBindFramebuffer(GL_FRAMEBUFFER, m_previewSceneFBO);

        m_gl->glBindTexture(GL_TEXTURE_2D, m_previewSceneTexture);
        if (viewportChanged) {
            m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, previewViewportWidth, previewViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }

        m_gl->glBindRenderbuffer(GL_RENDERBUFFER, m_previewSceneDepthRB);
        if (viewportChanged) {
            m_gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, previewViewportWidth, previewViewportHeight);
        }

        m_gl->glViewport(0, 0, previewViewportWidth, previewViewportHeight);

        m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderScene(*m_scene->previewCamera());

        // Render to the screen
        m_gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_gl->glViewport(0, 0, viewportWidth,viewportHeight);

        m_gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gl->glActiveTexture(GL_TEXTURE0);
        m_gl->glBindTexture(GL_TEXTURE_2D, m_sceneTexture);
        m_gl->glActiveTexture(GL_TEXTURE1);
        m_gl->glBindTexture(GL_TEXTURE_2D, m_previewSceneTexture);

        QOpenGLShaderProgram *sceneProgram = (*m_shaderPrograms)[ShaderPrograms::SceneProgram];
        sceneProgram->bind();
        sceneProgram->setUniformValue("u_sceneTexture", 0);
        sceneProgram->setUniformValue("u_previewSceneTexture", 1);
        sceneProgram->setUniformValue("u_previewSize", previewSize);
        m_quad->draw(*m_gl);
        sceneProgram->release();

        // Reset OpenGL state for qml
        // According to https://qt.gitorious.org/qt/qtdeclarative/source/fa0eea53f73c9b03b259f075e4cd5b83bfefccd3:src/quick/items/qquickwindow.cpp
        m_gl->glEnable(GL_TEXTURE_2D);
        m_gl->glActiveTexture(GL_TEXTURE0);
        m_gl->glBindTexture(GL_TEXTURE_2D, 0);
        m_gl->glDisable(GL_TEXTURE_2D);
        m_gl->glDisable(GL_DEPTH_TEST);
        m_gl->glClearColor(0, 0, 0, 0);
        m_gl->glDepthMask(GL_TRUE);
        m_gl->glDepthFunc(GL_LESS);

        QCoreApplication::postEvent(m_painterQML, new QEvent(m_painterQML->paintingDoneEventType()), Qt::HighEventPriority);
    }
}

void Painter::initialize()
{
    initializeRefractionEnvmap();
    initializeShaderPrograms();
    initializeFBOs();
    m_initialized = true;
}

void Painter::initializeFBOs()
{
    m_gl->glGenFramebuffers(1, &m_sceneFBO);
    m_gl->glBindFramebuffer(GL_FRAMEBUFFER, m_sceneFBO);

    m_gl->glGenRenderbuffers(1, &m_sceneDepthRB);
    m_gl->glBindRenderbuffer(GL_RENDERBUFFER, m_sceneDepthRB);

    m_gl->glGenTextures(1, &m_sceneTexture);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_sceneTexture);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    m_gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_sceneTexture, 0);
    m_gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_sceneDepthRB);
    m_gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if(m_gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }

    m_gl->glGenFramebuffers(1, &m_previewSceneFBO);
    m_gl->glBindFramebuffer(GL_FRAMEBUFFER, m_previewSceneFBO);

    m_gl->glGenRenderbuffers(1, &m_previewSceneDepthRB);
    m_gl->glBindRenderbuffer(GL_RENDERBUFFER, m_previewSceneDepthRB);

    m_gl->glGenTextures(1, &m_previewSceneTexture);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_previewSceneTexture);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    m_gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_previewSceneTexture, 0);
    m_gl->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_previewSceneDepthRB);
    m_gl->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if(m_gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }
}

void Painter::initializeShaderPrograms()
{
    QOpenGLShaderProgram *gemProgram = new QOpenGLShaderProgram(this);
    gemProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    gemProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!gemProgram->link()) {
        qDebug() << "Gem: Link failed";
    }

    gemProgram->bindAttributeLocation("a_vertex", 0);
    gemProgram->bindAttributeLocation("a_normal", 1);

    QOpenGLShaderProgram *lightRayProgram = new QOpenGLShaderProgram(this);
    lightRayProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/lightray.vert");
    lightRayProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/lightray.frag");

    if (!lightRayProgram->link()) {
        qDebug() << "LightRay: Link failed";
    }

    lightRayProgram->bindAttributeLocation("a_vertex", 0);

    m_shaderPrograms->insert(ShaderPrograms::GemProgram, gemProgram);
    m_shaderPrograms->insert(ShaderPrograms::LighRayProgram, lightRayProgram);

    initializeEnvmap();

    QOpenGLShaderProgram *sceneProgram = new QOpenGLShaderProgram(this);
    sceneProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/scene.vert");
    sceneProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/scene.frag");

    if (!sceneProgram->link()) {
        qDebug() << "Scene: Link failed";
    }

    sceneProgram->bindAttributeLocation("a_vertex", 0);

    m_shaderPrograms->insert(ShaderPrograms::SceneProgram, sceneProgram);
}

void Painter::initializeCubeMap(QString cubemapPrefix, uint &cubemapTexture)
{
    QMap<GLenum, QImage> images;
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_X] = QImage(":/data/" + cubemapPrefix + "_env_cube_px.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] = QImage(":/data/" + cubemapPrefix + "_env_cube_nx.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] = QImage(":/data/" + cubemapPrefix + "_env_cube_py.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] = QImage(":/data/" + cubemapPrefix + "_env_cube_ny.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] = QImage(":/data/" + cubemapPrefix + "_env_cube_pz.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] = QImage(":/data/" + cubemapPrefix + "_env_cube_nz.png").convertToFormat(QImage::Format_RGBA8888);

    m_gl->glGenTextures(1, &cubemapTexture);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //m_gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    QList<GLenum> faces = QList<GLenum>()
            << GL_TEXTURE_CUBE_MAP_POSITIVE_X << GL_TEXTURE_CUBE_MAP_NEGATIVE_X
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Y << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Z << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    foreach(GLenum face, faces) {
            const QImage &image(images[face]);
            m_gl->glTexImage2D(face, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }
}

void Painter::initializeEnvmap()
{
    // Initialize squad
    m_quad = new ScreenAlignedQuad();

    initializeCubeMap(m_envMapPrefix, m_envmap);

    QOpenGLShaderProgram *envmapProgram = new QOpenGLShaderProgram();
    envmapProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/envmap.vert");
    envmapProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/envmap.frag");

    if (!envmapProgram->link()) {
        qDebug() << "Light: Link failed";
    }

    envmapProgram->bindAttributeLocation("a_vertex", 0);

    m_shaderPrograms->insert(ShaderPrograms::EnvMapProgram, envmapProgram);
}

void Painter::initializeRefractionEnvmap()
{
    initializeCubeMap(QString::fromStdString("refraction"), m_refractionMap);
}

void Painter::paintEnvmap(const Camera &camera)
{
    QOpenGLShaderProgram *envmapProgram = (*m_shaderPrograms)[ShaderPrograms::EnvMapProgram];
    envmapProgram->bind();

    envmapProgram->setUniformValue("view",camera.view());
    envmapProgram->setUniformValue("projectionInverse", camera.projectionInverted());

    envmapProgram->setUniformValue("cubemap", 0);

    m_gl->glDepthMask(GL_FALSE);
    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glEnable(GL_TEXTURE_CUBE_MAP);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_envmap);

    envmapProgram->bind();
    m_quad->draw(*m_gl);
    envmapProgram->release();

    m_gl->glDepthMask(GL_TRUE);
    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_gl->glDisable(GL_TEXTURE_CUBE_MAP);
}

void Painter::renderScene(const Camera &camera)
{
    paintEnvmap(camera);

    /* Paint gems */
    QOpenGLShaderProgram *gemProgram = (*m_shaderPrograms)[ShaderPrograms::GemProgram];
    gemProgram->bind();

    gemProgram->enableAttributeArray(0);
    gemProgram->enableAttributeArray(1);

    gemProgram->setUniformValue("envmap", 0);
    gemProgram->setUniformValue("refractionMap", 1);
    gemProgram->setUniformValue("eye", camera.eye());
    gemProgram->setUniformValue("viewProjection", camera.viewProjection());
    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_envmap);
    m_gl->glActiveTexture(GL_TEXTURE1);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, m_refractionMap);

    QMap<ShaderPrograms, QOpenGLShaderProgram*> shaderPrograms;
    shaderPrograms.insert(ShaderPrograms::GemProgram, m_shaderPrograms->value(ShaderPrograms::GemProgram));
    shaderPrograms.insert(ShaderPrograms::EnvMapProgram, m_shaderPrograms->value(ShaderPrograms::EnvMapProgram));
    shaderPrograms.insert(ShaderPrograms::LighRayProgram, m_shaderPrograms->value(ShaderPrograms::LighRayProgram));

    m_scene->paint(*m_gl, camera.viewProjection(), *m_shaderPrograms);

    m_gl->glActiveTexture(GL_TEXTURE0);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_gl->glActiveTexture(GL_TEXTURE1);
    m_gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    gemProgram->disableAttributeArray(0);
    gemProgram->disableAttributeArray(1);

    gemProgram->release();
}

QString Painter::envMapPrefix() const
{
    return m_envMapPrefix;
}

void Painter::setEnvMapPrefix(const QString &envMapPrefix)
{
    m_envMapPrefix = envMapPrefix;
}

QOpenGLFunctions &Painter::gl() const
{
    return *m_gl;
}
