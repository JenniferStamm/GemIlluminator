#include "lightrayrenderer.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSet>
#include <QVector>

#include "lightray.h"
#include "lightraydata.h"

LightRayRenderer::LightRayRenderer(QObject *parent) :
    QObject(parent)
  , m_isStaticVBOUpdateRequired(true)
  , m_staticVertexBuffer(nullptr)
  , m_staticIndexBuffer(nullptr)
  , m_dynamicVertexBuffer(nullptr)
  , m_dynamicIndexBuffer(nullptr)
  , m_dynamicRays(new QSet<LightRayData>)
  , m_staticRays(new QSet<LightRayData>)
{
}

LightRayRenderer::~LightRayRenderer()
{
    delete m_staticVertexBuffer;
    delete m_staticIndexBuffer;
    delete m_dynamicVertexBuffer;
    delete m_dynamicIndexBuffer;
    delete m_dynamicRays;
    delete m_staticRays;
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
        m_dynamicRays->insert(LightRayData(ray));
    }
}

void LightRayRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram)
{
    if (m_isStaticVBOUpdateRequired) {
        updateStaticVBO();
    }

    shaderProgram.bind();
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setUniformValue("modelViewProjection", viewProjection);

    m_staticVertexBuffer->bind();
    m_staticIndexBuffer->bind();

    if (!m_staticRays->isEmpty()) {
        shaderProgram.setUniformValue("color", m_staticRays->values().first().color());
    }
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

    if (!m_dynamicRays->isEmpty()) {
        shaderProgram.setUniformValue("color", m_dynamicRays->values().first().color());
    }
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    for (int i = 0; i < m_dynamicIndexBuffer->size(); i += bytesPerLightRay) {
        gl.glDrawElements(GL_TRIANGLE_STRIP, indicesPerLightRay, GL_UNSIGNED_INT, reinterpret_cast<void *>(i));
    }

    m_dynamicVertexBuffer->release();
    m_dynamicIndexBuffer->release();

    shaderProgram.disableAttributeArray(0);
    shaderProgram.release();
}

void LightRayRenderer::resetDynamicRays()
{
    m_dynamicRays->clear();
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

void LightRayRenderer::updateRayVBO(
        QOpenGLBuffer *&vertexBuffer,
        QOpenGLBuffer *&indexBuffer,
        const QSet<LightRayData> &data)
{
    if (!vertexBuffer) {
        vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
        vertexBuffer->create();
    }
    if (!indexBuffer) {
        indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
        indexBuffer->create();
    }

    vertexBuffer->bind();
    indexBuffer->bind();

    QVector<float> vertexData;
    QVector<unsigned int> indexData;

    for (auto& rayData : data) {
        calculateVertexDataFor(rayData, vertexData, indexData);
    }

    vertexBuffer->allocate(vertexData.data(), vertexData.count() * sizeof(float));
    indexBuffer->allocate(indexData.data(), indexData.count() * sizeof(unsigned int));
}

void LightRayRenderer::updateDynamicVBO()
{
    updateRayVBO(m_dynamicVertexBuffer, m_dynamicIndexBuffer, *m_dynamicRays);
}

void LightRayRenderer::updateStaticVBO()
{
    updateRayVBO(m_staticVertexBuffer, m_staticIndexBuffer, *m_staticRays);
    m_isStaticVBOUpdateRequired = false;
}
