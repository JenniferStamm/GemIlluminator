#include "blureffect.h"

#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSize>

#include "screenalignedquad.h"
#include "shaderprograms.h"

BlurEffect::BlurEffect(QOpenGLFunctions &gl, uint blurTexture, QObject *parent) :
    QObject(parent)
  , m_gl(gl)
  , m_shaderPrograms(new QMap<ShaderPrograms, QOpenGLShaderProgram*>())
  , m_initialized(false)
  , m_blurTexture(blurTexture)
  , m_usedViewport(new QSize())
  , m_quad(nullptr)
{

}

BlurEffect::~BlurEffect()
{
    m_gl.glDeleteTextures(1, &m_secondaryBlurTexture);
    m_gl.glDeleteFramebuffers(1, &m_blurFBO);
    m_gl.glDeleteFramebuffers(1, &m_secondaryBlurFBO);

    delete m_quad;
    for (auto i : *m_shaderPrograms) {
        delete i;
    }
    delete m_shaderPrograms;
    delete m_usedViewport;
}

void BlurEffect::blur(const QSize &textureSize)
{
    if (!m_initialized) {
        initialize();
    }

    bool viewportChanged = false;
    if (*m_usedViewport != textureSize) {
        *m_usedViewport = textureSize;
        viewportChanged = true;
    }

    // Smooth lightRayTexture
    // Gauss Horizontal - lightRayTexture to secondaryLightRayTexture
    m_gl.glDisable(GL_DEPTH_TEST);
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_secondaryBlurFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryBlurTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.width(), textureSize.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    m_gl.glViewport(0, 0, textureSize.width(), textureSize.height());

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussHorizontal(textureSize);

    // Gauss Vertical - secondaryLightRayTexture to lightRayTexture
    m_gl.glBindFramebuffer(GL_FRAMEBUFFER, m_blurFBO);

    m_gl.glBindTexture(GL_TEXTURE_2D, m_blurTexture);
    if (viewportChanged) {
        m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.width(), textureSize.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    if (viewportChanged) {
        m_gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureSize.width(), textureSize.height());
    }
    m_gl.glViewport(0, 0, textureSize.width(), textureSize.height());

    m_gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGaussVertical(textureSize);

    m_gl.glEnable(GL_DEPTH_TEST);
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

    m_gl.glBindTexture(GL_TEXTURE_2D, m_blurTexture);
    m_gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    m_gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_blurTexture, 0);

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

void BlurEffect::renderGaussHorizontal(const QSize &textureSize)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussHorizontalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("u_source", 0);
    float blurSize = 1.0 / textureSize.width();
    shaderProgram->setUniformValue("u_blurSize", blurSize);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_blurTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}

void BlurEffect::renderGaussVertical(const QSize &textureSize)
{
    auto shaderProgram = m_shaderPrograms->value(ShaderPrograms::GaussVerticalProgram);
    shaderProgram->bind();

    shaderProgram->setUniformValue("u_source", 0);
    float blurSize = 1.0 / textureSize.height();
    shaderProgram->setUniformValue("u_blurSize", blurSize);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, m_secondaryBlurTexture);

    m_quad->draw(m_gl);

    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_2D, 0);

    shaderProgram->release();
}
