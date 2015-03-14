#include "environmentmap.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "camera.h"
#include "cubemap.h"
#include "screenalignedquad.h"

EnvironmentMap::EnvironmentMap(QOpenGLFunctions &gl, QString cubeMapPrefix, QObject *parent) :
    QObject(parent)
  , m_cubeMap(new CubeMap(gl, cubeMapPrefix))
  , m_initialized(false)
  , m_gl(gl)
  , m_quad(nullptr)
  , m_shaderProgram(new QOpenGLShaderProgram())
{

}

EnvironmentMap::~EnvironmentMap()
{
    delete m_quad;
    delete m_shaderProgram;
    delete m_cubeMap;
}

void EnvironmentMap::paint(const Camera &camera)
{
    if (!m_initialized) {
        initialize();
    }

    m_shaderProgram->bind();

    m_shaderProgram->setUniformValue("view",camera.view());
    m_shaderProgram->setUniformValue("projectionInverse", camera.projectionInverted());

    m_shaderProgram->setUniformValue("cubemap", 0);

    m_gl.glDepthMask(GL_FALSE);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glEnable(GL_TEXTURE_CUBE_MAP);
    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap->cubeMapTexture());

    m_shaderProgram->bind();
    m_quad->draw(m_gl);
    m_shaderProgram->release();

    m_gl.glDepthMask(GL_TRUE);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_gl.glDisable(GL_TEXTURE_CUBE_MAP);
}

void EnvironmentMap::update(QString newCubeMapPrefix)
{
    m_cubeMap->update(newCubeMapPrefix);
}

uint EnvironmentMap::cubeMapTexture()
{
    return m_cubeMap->cubeMapTexture();
}

void EnvironmentMap::initialize()
{
    // Initialize squad
    m_quad = new ScreenAlignedQuad();

    initializeShaderProgram();

    m_initialized = true;
}

void EnvironmentMap::initializeShaderProgram()
{
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/envmap.vert");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/envmap.frag");

    if (!m_shaderProgram->link()) {
        qDebug() << "Envmap: Link failed";
    }

    m_shaderProgram->bindAttributeLocation("a_vertex", 0);
}
