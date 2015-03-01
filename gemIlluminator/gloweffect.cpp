#include "gloweffect.h"

#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSize>

#include "camera.h"
#include "screenalignedquad.h"
#include "scene.h"

GlowEffect::GlowEffect(QOpenGLFunctions &gl, const Camera &camera, QObject *parent) :
    QObject(parent)
  , m_camera(camera)
  , m_gl(gl)
  , m_shaderPrograms(new QMap<ShaderPrograms, QOpenGLShaderProgram*>())
  , m_initialized(false)
  , m_usedViewport(new QSize())
  , m_quad(nullptr)
{

}

GlowEffect::~GlowEffect()
{
    m_gl.glDeleteTextures(1, &m_lightRayTexture);
    m_gl.glDeleteTextures(1, &m_secondaryLightRayTexture);
    m_gl.glDeleteRenderbuffers(1, &m_lightRayDepthRB);
    m_gl.glDeleteFramebuffers(1, &m_lightRayFBO);
    m_gl.glDeleteFramebuffers(1, &m_secondaryLightRayFBO);

    delete m_quad;
    for (auto i : *m_shaderPrograms) {
        delete i;
    }
    delete m_shaderPrograms;
    delete m_usedViewport;
}

void GlowEffect::renderGlowToTexture(uint glowTexture)
{
    if (!m_initialized) {
        initialize();
    }

    m_lightRayTexture = glowTexture;


    int viewportHeight = m_camera.viewport().height();
    int viewportWidth = m_camera.viewport().width();
    int glowViewportRatio = 4;
    int glowViewportHeight = viewportHeight / glowViewportRatio;
    int glowViewportWidth = viewportWidth / glowViewportRatio;

    bool viewportChanged = false;
    if (m_usedViewport->height() != viewportHeight
            && m_usedViewport->width() != viewportWidth) {
        *m_usedViewport = m_camera.viewport();
        viewportChanged = true;
    }

    // Smooth lightRayTexture
    // Gauss Horizontal - lightRayTexture to secondaryLightRayTexture
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_secondaryLightRayFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryLightRayTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowViewportWidth, glowViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_lightRayDepthRB);
    if (viewportChanged) {
        m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, glowViewportWidth, glowViewportHeight);
    }
    m_gl.glViewport(0, 0, glowViewportWidth, glowViewportHeight);

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussHorizontal(m_camera);

    // Gauss Vertical - secondaryLightRayTexture to lightRayTexture
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_lightRayFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_lightRayTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowViewportWidth, glowViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_lightRayDepthRB);
    if (viewportChanged) {
        m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, glowViewportWidth, glowViewportHeight);
    }
    m_gl.glViewport(0, 0, glowViewportWidth, glowViewportHeight);

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussVertical(m_camera);

    glowTexture = m_lightRayTexture;
}

void GlowEffect::initialize()
{
    m_quad = new ScreenAlignedQuad();
    initializeShaderPrograms();
    initializeFBOs();
    m_initialized = true;
}

void GlowEffect::initializeFBOs()
{
    m_gl.glGenFramebuffers(1, &m_lightRayFBO);
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_lightRayFBO);

    m_gl.glGenRenderbuffers(1, &m_lightRayDepthRB);
    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_lightRayDepthRB);

    m_gl.glGenTextures(1, &m_lightRayTexture);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_lightRayTexture);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    m_gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightRayTexture, 0);
    m_gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRayDepthRB);
    m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if (m_gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }

    m_gl.glGenFramebuffers(1, &m_secondaryLightRayFBO);
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_secondaryLightRayFBO);

    m_gl.glGenTextures(1, &m_secondaryLightRayTexture);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryLightRayTexture);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    m_gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_secondaryLightRayTexture, 0);
    m_gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_lightRayDepthRB);
    m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if (m_gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }
}

void GlowEffect::initializeShaderPrograms()
{
    auto gaussHorizontalProgram = new QOpenGLShaderProgram(this);
    gaussHorizontalProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/screenquad.vert");
    gaussHorizontalProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/gausshorizontal.frag");
    if (!gaussHorizontalProgram->link()) {
        qDebug() << "Gauss horizontal: Link failed";
    }
    gaussHorizontalProgram->bindAttributeLocation("a_vertex", 0);
    m_shaderPrograms->insert(ShaderPrograms::GaussHorizontalProgram, gaussHorizontalProgram);

    auto gaussVerticalProgram = new QOpenGLShaderProgram(this);
    gaussVerticalProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/screenquad.vert");
    gaussVerticalProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/gaussvertical.frag");
    if (!gaussVerticalProgram->link()) {
        qDebug() << "Gauss horizontal: Link failed";
    }
    gaussVerticalProgram->bindAttributeLocation("a_vertex", 0);
    m_shaderPrograms->insert(ShaderPrograms::GaussVerticalProgram, gaussVerticalProgram);
}

void GlowEffect::renderGaussHorizontal(const Camera &camera)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussHorizontalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("view",camera.view());
    shaderProgram->setUniformValue("projectionInverse", camera.projectionInverted());
    shaderProgram->setUniformValue("lightRays", 0);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_lightRayTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}

void GlowEffect::renderGaussVertical(const Camera &camera)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussVerticalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("view",camera.view());
    shaderProgram->setUniformValue("projectionInverse", camera.projectionInverted());
    shaderProgram->setUniformValue("lightRays", 0);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryLightRayTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}





