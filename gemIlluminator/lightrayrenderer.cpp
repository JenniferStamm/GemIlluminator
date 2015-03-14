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
  , m_dynamicIndexBuffer(nullptr)
  , m_dynamicRays(new QSet<LightRayData>)
  , m_dynamicVertexBuffer(nullptr)
  , m_isStaticVBOUpdateRequired(true)
  , m_staticIndexBuffer(nullptr)
  , m_staticRays(new QSet<LightRayData>)
  , m_staticVertexBuffer(nullptr)
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
    shaderProgram.enableAttributeArray(1);
    shaderProgram.setUniformValue("u_modelViewProjection", viewProjection);

    m_staticVertexBuffer->bind();
    m_staticIndexBuffer->bind();

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    gl.glDrawElements(GL_TRIANGLE_STRIP, m_staticIndexBuffer->size(), GL_UNSIGNED_INT, nullptr);

    m_staticVertexBuffer->release();
    m_staticIndexBuffer->release();

    updateDynamicVBO();
    m_dynamicVertexBuffer->bind();
    m_dynamicIndexBuffer->bind();

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    gl.glDrawElements(GL_TRIANGLE_STRIP, m_dynamicIndexBuffer->size(), GL_UNSIGNED_INT, nullptr);

    m_dynamicVertexBuffer->release();
    m_dynamicIndexBuffer->release();

    shaderProgram.disableAttributeArray(0);
    shaderProgram.release();
}

void LightRayRenderer::resetDynamicRays()
{
    m_dynamicRays->clear();
}

void pushVertexAndColorToVector(const QVector3D &pos, const QVector3D &color, QVector<float> &vector)
{
    vector.push_back(pos.x());
    vector.push_back(pos.y());
    vector.push_back(pos.z());
    vector.push_back(color.x());
    vector.push_back(color.y());
    vector.push_back(color.z());
}

void LightRayRenderer::calculateVertexDataFor(const LightRayData & rayData, QVector<float> &vertices, QVector<unsigned int> & indices)
{
    float offset = 0.01f;
    QVector3D direction = (rayData.endPosition() - rayData.startPosition()).normalized();
    QVector3D upVector = rayData.normalizedOrthogonalVector();
    QVector3D rightVector = QVector3D::crossProduct(direction, upVector).normalized();

    //s-start, e-end, t-top, b-bottom, r-right, l-left
    int valuesPerVertex = 6;
    unsigned int startTop = vertices.count() / valuesPerVertex;
    unsigned int startRight =  startTop + 1;
    unsigned int startBottom = startTop + 2;
    unsigned int startLeft =   startTop + 3;
    unsigned int endTop =      startTop + 4;
    unsigned int endRight =    startTop + 5;
    unsigned int endBottom =   startTop + 6;
    unsigned int endLeft =     startTop + 7;

    pushVertexAndColorToVector(rayData.startPosition() + upVector    * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.startPosition() + rightVector * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.startPosition() - upVector    * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.startPosition() - rightVector * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.endPosition()   + upVector    * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.endPosition()   + rightVector * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.endPosition()   - upVector    * offset, rayData.color(), vertices);
    pushVertexAndColorToVector(rayData.endPosition()   - rightVector * offset, rayData.color(), vertices);

    indices.push_back(startBottom);
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
