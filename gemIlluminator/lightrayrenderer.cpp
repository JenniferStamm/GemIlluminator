#include "lightrayrenderer.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSet>
#include <QVector>

#include "camera.h"
#include "lightray.h"
#include "lightraydata.h"

LightRayRenderer::LightRayRenderer(QObject *parent) :
    QObject(parent)
  , m_isInitialized(false)
  , m_program(nullptr)
  , m_camera(nullptr)
  , m_dynamicRays(new QVector<LightRay *>)
  , m_staticRays(new QSet<LightRayData>)
  , m_isStaticVBOUpdateRequired(true)
  , m_vertexBuffer(nullptr)
  , m_indexBuffer(nullptr)
{
}

LightRayRenderer::~LightRayRenderer()
{
    delete m_program;
    delete m_vertexBuffer;
    delete m_dynamicRays;
    delete m_staticRays;
}

void LightRayRenderer::setCamera(Camera & camera)
{
    delete m_camera;
    m_camera = new Camera(camera, this);
}

void LightRayRenderer::addLightRay(const LightRay & ray)
{
    if (ray.isStatic()) {
        LightRayData temp(ray);
        if (m_staticRays->contains(temp)) {
            return;
        }
        m_staticRays->insert(temp);
        m_isStaticVBOUpdateRequired = true;
    }
    else
    {
        //m_dynamicRays->push_back(ray);
    }
}

void LightRayRenderer::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/lightray.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/lightray.frag");
    if (!m_program->link()) {
        qDebug() << "LightRay: Link failed";
    }

    m_program->bindAttributeLocation("a_vertex", 0);

    m_isInitialized = true;
}

QVector3D calculateOrthogonalVectorTo(const QVector3D &vector)
{
    if (vector.x() != 0.f) {
        if (vector.z() != 0.f) {
            return QVector3D(1.f, 0.f, - vector.x() / vector.z()).normalized();
        } else {
            return QVector3D(0.f, 1.f, 0.f);
        }
    } else if (vector.y() != 0.f) {
        if (vector.z() != 0.f) {
            return QVector3D(0.f, 1.f, - vector.y() / vector.z()).normalized();
        } else {
            return QVector3D(1.f, 0.f, 0.f);
        }
    } else if (vector.z() != 0.f) {
        return QVector3D(1.f, 0.f, 0.f);
    } else {
        return QVector3D(0.f, 0.f, 0.f);
    }
}

void LightRayRenderer::updateStaticVBO()
{
    if (m_vertexBuffer) {
        m_vertexBuffer->destroy();
    }
    if (m_indexBuffer) {
        m_indexBuffer->destroy();
    }
    delete m_vertexBuffer;
    delete m_indexBuffer;
    m_vertexBuffer = new QOpenGLBuffer();
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_vertexBuffer->create();
    m_indexBuffer->create();
    m_vertexBuffer->bind();
    m_indexBuffer->bind();

    QVector<float> vertexData;
    QVector<unsigned int> indexData;

    for (auto& rayData : *m_staticRays) {
        float offset = 0.01f;
        QVector3D direction = (rayData.endPosition() - rayData.startPosition()).normalized();
        QVector3D upVector = calculateOrthogonalVectorTo(direction);
        QVector3D rightVector = QVector3D::crossProduct(direction, upVector).normalized();

        //s-start, e-end, t-top, b-bottom, r-right, l-left
        unsigned int startTop = vertexData.count();
        unsigned int startRight =  startTop + 1;
        unsigned int startBottom = startTop + 2;
        unsigned int startLeft =   startTop + 3;
        unsigned int endTop =      startTop + 4;
        unsigned int endRight =    startTop + 5;
        unsigned int endBottom =   startTop + 6;
        unsigned int endLeft =     startTop + 7;

        vertexData.push_back((rayData.startPosition() + upVector * offset).x());
        vertexData.push_back((rayData.startPosition() + upVector * offset).y());
        vertexData.push_back((rayData.startPosition() + upVector * offset).z());
        vertexData.push_back((rayData.startPosition() + rightVector * offset).x());
        vertexData.push_back((rayData.startPosition() + rightVector * offset).y());
        vertexData.push_back((rayData.startPosition() + rightVector * offset).z());
        vertexData.push_back((rayData.startPosition() - upVector * offset).x());
        vertexData.push_back((rayData.startPosition() - upVector * offset).y());
        vertexData.push_back((rayData.startPosition() - upVector * offset).z());
        vertexData.push_back((rayData.startPosition() - rightVector * offset).x());
        vertexData.push_back((rayData.startPosition() - rightVector * offset).y());
        vertexData.push_back((rayData.startPosition() - rightVector * offset).z());
        vertexData.push_back((rayData.endPosition() + upVector * offset).x());
        vertexData.push_back((rayData.endPosition() + upVector * offset).y());
        vertexData.push_back((rayData.endPosition() + upVector * offset).z());
        vertexData.push_back((rayData.endPosition() + rightVector * offset).x());
        vertexData.push_back((rayData.endPosition() + rightVector * offset).y());
        vertexData.push_back((rayData.endPosition() + rightVector * offset).z());
        vertexData.push_back((rayData.endPosition() - upVector * offset).x());
        vertexData.push_back((rayData.endPosition() - upVector * offset).y());
        vertexData.push_back((rayData.endPosition() - upVector * offset).z());
        vertexData.push_back((rayData.endPosition() - rightVector * offset).x());
        vertexData.push_back((rayData.endPosition() - rightVector * offset).y());
        vertexData.push_back((rayData.endPosition() - rightVector * offset).z());

        indexData.push_back(startBottom);
        indexData.push_back(startRight);
        indexData.push_back(startLeft);
        indexData.push_back(startTop);
        indexData.push_back(endTop);
        indexData.push_back(startRight);
        indexData.push_back(endRight);
        indexData.push_back(endBottom);
        indexData.push_back(endTop);
        indexData.push_back(endLeft);
        indexData.push_back(startLeft);
        indexData.push_back(endBottom);
        indexData.push_back(startBottom);
        indexData.push_back(startRight);
    }

    m_vertexBuffer->allocate(vertexData.data(), vertexData.count() * 3 * sizeof(float));
    m_indexBuffer->allocate(indexData.data(), indexData.count() * sizeof(unsigned int));
    m_isStaticVBOUpdateRequired = false;
}

void LightRayRenderer::paint(QOpenGLFunctions *gl)
{
    if (!m_isInitialized) {
        initialize();
    }

    if (m_isStaticVBOUpdateRequired) {
        updateStaticVBO();
    }

    m_vertexBuffer->bind();
    m_indexBuffer->bind();

    m_program->bind();
    m_program->enableAttributeArray(0);
    m_program->setUniformValue("modelViewProjection", m_camera->viewProjection());

    gl->glDisable(GL_CULL_FACE);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    gl->glDrawElements(GL_TRIANGLE_STRIP, m_indexBuffer->size() / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer->release();
    m_indexBuffer->release();

    m_program->disableAttributeArray(0);
    m_program->release();

    m_dynamicRays->clear();
}
