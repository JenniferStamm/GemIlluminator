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
  , m_isStaticVBOUpdateRequired(true)
  , m_program(nullptr)
  , m_staticVertexBuffer(nullptr)
  , m_staticIndexBuffer(nullptr)
  , m_dynamicVertexBuffer(nullptr)
  , m_dynamicIndexBuffer(nullptr)
  , m_camera(nullptr)
  , m_dynamicRays(new QVector<LightRayData>)
  , m_staticRays(new QSet<LightRayData>)
{
}

LightRayRenderer::~LightRayRenderer()
{
    delete m_program;
    delete m_staticVertexBuffer;
    delete m_staticIndexBuffer;
    delete m_dynamicVertexBuffer;
    delete m_dynamicIndexBuffer;
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
        m_dynamicRays->push_back(LightRayData(ray));
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

void LightRayRenderer::calculateVertexDataFor(const LightRayData & rayData, QVector<float> &vertices, QVector<unsigned int> & indices)
{
    float offset = 0.01f;
    QVector3D direction = (rayData.endPosition() - rayData.startPosition()).normalized();
    QVector3D upVector = rayData.normalizedOrthogonalVector();
    QVector3D rightVector = QVector3D::crossProduct(direction, upVector).normalized();

    //s-start, e-end, t-top, b-bottom, r-right, l-left
    int valuesPerVertex = 3;
    unsigned int startTop = vertices.count() / valuesPerVertex;
    unsigned int startRight =  startTop + 1;
    unsigned int startBottom = startTop + 2;
    unsigned int startLeft =   startTop + 3;
    unsigned int endTop =      startTop + 4;
    unsigned int endRight =    startTop + 5;
    unsigned int endBottom =   startTop + 6;
    unsigned int endLeft =     startTop + 7;

    vertices.push_back((rayData.startPosition() + upVector * offset).x());
    vertices.push_back((rayData.startPosition() + upVector * offset).y());
    vertices.push_back((rayData.startPosition() + upVector * offset).z());
    vertices.push_back((rayData.startPosition() + rightVector * offset).x());
    vertices.push_back((rayData.startPosition() + rightVector * offset).y());
    vertices.push_back((rayData.startPosition() + rightVector * offset).z());
    vertices.push_back((rayData.startPosition() - upVector * offset).x());
    vertices.push_back((rayData.startPosition() - upVector * offset).y());
    vertices.push_back((rayData.startPosition() - upVector * offset).z());
    vertices.push_back((rayData.startPosition() - rightVector * offset).x());
    vertices.push_back((rayData.startPosition() - rightVector * offset).y());
    vertices.push_back((rayData.startPosition() - rightVector * offset).z());
    vertices.push_back((rayData.endPosition() + upVector * offset).x());
    vertices.push_back((rayData.endPosition() + upVector * offset).y());
    vertices.push_back((rayData.endPosition() + upVector * offset).z());
    vertices.push_back((rayData.endPosition() + rightVector * offset).x());
    vertices.push_back((rayData.endPosition() + rightVector * offset).y());
    vertices.push_back((rayData.endPosition() + rightVector * offset).z());
    vertices.push_back((rayData.endPosition() - upVector * offset).x());
    vertices.push_back((rayData.endPosition() - upVector * offset).y());
    vertices.push_back((rayData.endPosition() - upVector * offset).z());
    vertices.push_back((rayData.endPosition() - rightVector * offset).x());
    vertices.push_back((rayData.endPosition() - rightVector * offset).y());
    vertices.push_back((rayData.endPosition() - rightVector * offset).z());

    indices.push_back(startBottom);
    indices.push_back(startRight);
    indices.push_back(startLeft);
    indices.push_back(startTop);
    indices.push_back(endTop);
    indices.push_back(startRight);
    indices.push_back(endRight);
    indices.push_back(endBottom);
    indices.push_back(endTop);
    indices.push_back(endLeft);
    indices.push_back(startLeft);
    indices.push_back(endBottom);
    indices.push_back(startBottom);
    indices.push_back(startRight);
}

void LightRayRenderer::updateStaticVBO()
{
    if (!m_staticVertexBuffer) {
        m_staticVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
        m_staticVertexBuffer->create();
    }
    if (!m_staticIndexBuffer) {
        m_staticIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
        m_staticIndexBuffer->create();
    }
    m_staticVertexBuffer->bind();
    m_staticIndexBuffer->bind();

    QVector<float> vertexData;
    QVector<unsigned int> indexData;

    for (auto& rayData : *m_staticRays) {
        calculateVertexDataFor(rayData, vertexData, indexData);
    }

    m_staticVertexBuffer->allocate(vertexData.data(), vertexData.count() * sizeof(float));
    m_staticIndexBuffer->allocate(indexData.data(), indexData.count() * sizeof(unsigned int));
    m_isStaticVBOUpdateRequired = false;
}

void LightRayRenderer::updateDynamicVBO()
{
    if (!m_dynamicVertexBuffer) {
        m_dynamicVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
        m_dynamicVertexBuffer->create();
    }
    if (!m_dynamicIndexBuffer) {
        m_dynamicIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
        m_dynamicIndexBuffer->create();
    }
    m_dynamicVertexBuffer->bind();
    m_dynamicIndexBuffer->bind();

    QVector<float> vertexData;
    QVector<unsigned int> indexData;

    for (auto& rayData : *m_dynamicRays) {
        calculateVertexDataFor(rayData, vertexData, indexData);
    }

    m_dynamicVertexBuffer->allocate(vertexData.data(), vertexData.count() * sizeof(float));
    m_dynamicIndexBuffer->allocate(indexData.data(), indexData.count() * sizeof(unsigned int));
}

void LightRayRenderer::paint(QOpenGLFunctions &gl)
{
    if (!m_isInitialized) {
        initialize();
    }

    if (m_isStaticVBOUpdateRequired) {
        updateStaticVBO();
    }

    m_program->bind();
    m_program->enableAttributeArray(0);
    m_program->setUniformValue("modelViewProjection", m_camera->viewProjection());

    m_staticVertexBuffer->bind();
    m_staticIndexBuffer->bind();
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    int indicesPerLightRay = 14;
    int bytesPerLightRay = indicesPerLightRay * sizeof(unsigned int);
    for (int i = 0; i < m_staticIndexBuffer->size(); i += bytesPerLightRay) {
        gl.glDrawElements(GL_TRIANGLE_STRIP, indicesPerLightRay, GL_UNSIGNED_INT, reinterpret_cast<void *>(i));
    }

    m_staticVertexBuffer->release();
    m_staticIndexBuffer->release();

    updateDynamicVBO();
    m_dynamicVertexBuffer->bind();
    m_dynamicIndexBuffer->bind();

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    for (int i = 0; i < m_dynamicIndexBuffer->size(); i += bytesPerLightRay) {
        gl.glDrawElements(GL_TRIANGLE_STRIP, indicesPerLightRay, GL_UNSIGNED_INT, reinterpret_cast<void *>(i));
    }

    m_dynamicVertexBuffer->release();
    m_dynamicIndexBuffer->release();

    m_program->disableAttributeArray(0);
    m_program->release();

    m_dynamicRays->clear();
}
