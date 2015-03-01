#include "blureffect.h"

#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSize>

#include "camera.h"
#include "screenalignedquad.h"
#include "scene.h"

BlurEffect::BlurEffect(QOpenGLFunctions &gl, uint blurTexture, int viewportRatio, QObject *parent) :
    QObject(parent)
  , m_gl(gl)
  , m_shaderPrograms(new QMap<ShaderPrograms, QOpenGLShaderProgram*>())
  , m_initialized(false)
  , m_viewportRatio(viewportRatio)
  , m_blurTexture(blurTexture)
  , m_usedViewport(new QSize())
  , m_quad(nullptr)
{

}

BlurEffect::~BlurEffect()
{
    m_gl.glDeleteTextures(1, &m_secondaryBlurTexture);
    m_gl.glDeleteRenderbuffers(1, &m_blurDepthRB);
    m_gl.glDeleteFramebuffers(1, &m_blurFBO);
    m_gl.glDeleteFramebuffers(1, &m_secondaryBlurFBO);

    delete m_quad;
    for (auto i : *m_shaderPrograms) {
        delete i;
    }
    delete m_shaderPrograms;
    delete m_usedViewport;
}

void BlurEffect::renderGlowToTexture(const Camera &camera)
{
    if (!m_initialized) {
        initialize();
    }

    int viewportHeight = camera.viewport().height();
    int viewportWidth = camera.viewport().width();

    int glowViewportHeight = viewportHeight / m_viewportRatio;
    int glowViewportWidth = viewportWidth / m_viewportRatio;

    bool viewportChanged = false;
    if (m_usedViewport->height() != viewportHeight
            && m_usedViewport->width() != viewportWidth) {
        *m_usedViewport = camera.viewport();
        viewportChanged = true;
    }

    // Smooth lightRayTexture
    // Gauss Horizontal - lightRayTexture to secondaryLightRayTexture
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_secondaryBlurFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryBlurTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowViewportWidth, glowViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_blurDepthRB);
    if (viewportChanged) {
        m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, glowViewportWidth, glowViewportHeight);
    }
    m_gl.glViewport(0, 0, glowViewportWidth, glowViewportHeight);

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussHorizontal(camera, glowViewportWidth);

    // Gauss Vertical - secondaryLightRayTexture to lightRayTexture
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_blurTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glowViewportWidth, glowViewportHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_blurDepthRB);
    if (viewportChanged) {
        m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, glowViewportWidth, glowViewportHeight);
    }
    m_gl.glViewport(0, 0, glowViewportWidth, glowViewportHeight);

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussVertical(camera, glowViewportHeight);
}

void BlurEffect::initialize()
{
    m_quad = new ScreenAlignedQuad();
    initializeShaderPrograms();
    initializeFBOs();
    m_initialized = true;
}

void BlurEffect::initializeFBOs()
{
    m_gl.glGenFramebuffers(1, &m_blurFBO);
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);

    m_gl.glGenRenderbuffers(1, &m_blurDepthRB);
    m_gl.glBindRenderbuffer(GL_RENDERBUFFER, m_blurDepthRB);

    m_gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTexture, 0);
    m_gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_blurDepthRB);
    m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if (m_gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }

    m_gl.glGenFramebuffers(1, &m_secondaryBlurFBO);
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_secondaryBlurFBO);

    m_gl.glGenTextures(1, &m_secondaryBlurTexture);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryBlurTexture);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    m_gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_secondaryBlurTexture, 0);
    m_gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_blurDepthRB);
    m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1, 1);

    if (m_gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return;
    }
}

void BlurEffect::initializeShaderPrograms()
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

void BlurEffect::renderGaussHorizontal(const Camera &camera, int viewportWidth)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussHorizontalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("view", camera.view());
    shaderProgram->setUniformValue("projectionInverse", camera.projectionInverted());
    shaderProgram->setUniformValue("lightRays", 0);
    float blurSize = 1.0 / viewportWidth;
    shaderProgram->setUniformValue("blurSize", blurSize);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_blurTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}

void BlurEffect::renderGaussVertical(const Camera &camera, int viewportHeight)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussVerticalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("view", camera.view());
    shaderProgram->setUniformValue("projectionInverse", camera.projectionInverted());
    shaderProgram->setUniformValue("lightRays", 0);
    float blurSize = 1.0 / viewportHeight;
    shaderProgram->setUniformValue("blurSize", blurSize);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryBlurTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}






