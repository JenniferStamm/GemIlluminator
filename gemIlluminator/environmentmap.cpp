#include "environmentmap.h"


#include <QImage>
#include <QMap>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "camera.h"
#include "config.h"
#include "screenalignedquad.h"

EnvironmentMap::EnvironmentMap(QOpenGLFunctions &gl, QObject *parent) :
    QObject(parent)
  , m_gl(gl)
  , m_initialized(false)
  , m_shaderProgram(new QOpenGLShaderProgram())
{

}

EnvironmentMap::~EnvironmentMap()
{
    delete m_quad;

    m_gl.glDeleteTextures(1, &m_envMap);

    delete m_shaderProgram;
}

void EnvironmentMap::initialize()
{
    // Initialize squad
    m_quad = new ScreenAlignedQuad();

    // Initialize Cube Map
    QMap<GLenum, QImage> images;
    QString envMapPrefix = Config::instance()->envMap();
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_X] = QImage(":/data/" + envMapPrefix + "_env_cube_px.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_X] = QImage(":/data/" + envMapPrefix + "_env_cube_nx.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Y] = QImage(":/data/" + envMapPrefix + "_env_cube_py.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y] = QImage(":/data/" + envMapPrefix + "_env_cube_ny.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_POSITIVE_Z] = QImage(":/data/" + envMapPrefix + "_env_cube_pz.png").convertToFormat(QImage::Format_RGBA8888);
    images[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z] = QImage(":/data/" + envMapPrefix + "_env_cube_nz.png").convertToFormat(QImage::Format_RGBA8888);

    m_gl.glGenTextures(1, &m_envMap);
    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_envMap);

    m_gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    m_gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gl.glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_envMap);

    QList<GLenum> faces = QList<GLenum>()
            << GL_TEXTURE_CUBE_MAP_POSITIVE_X << GL_TEXTURE_CUBE_MAP_NEGATIVE_X
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Y << GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            << GL_TEXTURE_CUBE_MAP_POSITIVE_Z << GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    for (GLenum face: faces) {
            const QImage &image(images[face]);
            m_gl.glTexImage2D(face, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }

    initializeShaderProgram();

    m_initialized = true;
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
    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, m_envMap);

    m_shaderProgram->bind();
    m_quad->draw(m_gl);
    m_shaderProgram->release();

    m_gl.glDepthMask(GL_TRUE);
    m_gl.glActiveTexture(GL_TEXTURE0);
    m_gl.glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    m_gl.glDisable(GL_TEXTURE_CUBE_MAP);
}

uint EnvironmentMap::envMapTexture()
{
    return m_envMap;
}

void EnvironmentMap::initializeShaderProgram()
{
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/screenquad.vert");
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/envmap.frag");

    if (!m_shaderProgram->link()) {
        qDebug() << "Envmap: Link failed";
    }

    m_shaderProgram->bindAttributeLocation("a_vertex", 0);
}

