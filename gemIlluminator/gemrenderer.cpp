#include "gemrenderer.h"

#include <QHash>
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPair>
#include <QVector>
#include <QVector3D>

#include "abstractgem.h"
#include "gemdata.h"
#include "triangle.h"

GemRenderer::GemRenderer() :
    m_isInitialized(false)
  , m_gemMap(new QHash<AbstractGem *, GemDataInfo *>())
  , m_isGemBufferUpdateRequired(false)
  , m_newGems(new QList<GemDataInfo *>())
  , m_sceneExtent(1.f)
  , m_isGemDataBufferInvalid(false)
  , m_gemBuffersTex(new QHash<GemType, GemRenderData *>())
{
}

GemRenderer::~GemRenderer()
{
    //for (auto gem : m_gemMap->values()) {
    //    delete gem;
    //}
    //delete m_gemMap;

    for (auto renderData : m_gemBuffersTex->values()) {
        delete renderData;
    }
    delete m_gemBuffersTex;
}

void GemRenderer::cleanup(QOpenGLFunctions &gl)
{
    //TODO
}

void GemRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_isInitialized) {
        initialize(gl);
    }
    paintGemsOptimizedWithTexture(gl, viewProjection, program);
}

void GemRenderer::setSceneExtent(float extent)
{
    m_sceneExtent = extent;
    m_isGemDataBufferInvalid = true;
}

void GemRenderer::updateGem(AbstractGem *gem)
{
    updateGemForTextureOptimization(gem);
}

void GemRenderer::initialize(QOpenGLFunctions &gl)
{
    m_isInitialized = true;
    //gl.glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxUniformVectorSize);
}

void GemRenderer::paintGemsOptimizedWithTexture(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_newGems->isEmpty()) {
        for (auto gem : *m_newGems) {
            GemRenderData *typeRenderData = m_gemBuffersTex->value(gem->data().type());
            if (!typeRenderData) {
                typeRenderData = new GemRenderData();
                typeRenderData->setVerticesPerGem(gem->numberOfVertices());
                typeRenderData->setSceneExtent(m_sceneExtent);
                m_gemBuffersTex->insert(gem->data().type(), typeRenderData);
            }
            typeRenderData->addOrUpdateGem(gem, gl);
        }

        m_newGems->clear();
    }
    program.enableAttributeArray(2);
    for (auto renderData : m_gemBuffersTex->values()) {
        renderData->paint(gl, program);
    }
}

void GemRenderer::updateGemForTextureOptimization(AbstractGem *gem)
{
    GemDataInfo *gemInfo = m_gemMap->value(gem);
    if (gemInfo) {
        if (gemInfo->data() != gem->data()) {
            gemInfo->setData(gem->data());
            m_newGems->append(gemInfo);
        }
    } else {
        gemInfo = new GemDataInfo();
        gemInfo->setData(gem->data());
        m_gemMap->insert(gem, gemInfo);
        m_newGems->append(gemInfo);
    }
}

GemRenderer::GemDataInfo::GemDataInfo() :
    m_data(new GemData())
{
}

GemRenderer::GemDataInfo::~GemDataInfo()
{
    delete m_data;
}

const GemData &GemRenderer::GemDataInfo::data() const
{
    return *m_data;
}

void GemRenderer::GemDataInfo::setData(const GemData &data)
{
    *m_data = data;
}

int GemRenderer::GemDataInfo::index() const
{
    return m_index;
}

void GemRenderer::GemDataInfo::setIndex(int index)
{
    m_index = index;
}

int GemRenderer::GemDataInfo::numberOfVertices()
{
    return m_data->triangles().count() * 3;
}

void GemRenderer::GemDataInfo::appendVerticesWithIndexTo(QVector<float> &vector) const
{
    for (auto triangle : m_data->triangles()) {
        for (auto& vertex : triangle->vertices()) {
            vector.append(vertex.x());
            vector.append(vertex.y());
            vector.append(vertex.z());
            vector.append(triangle->normal().x());
            vector.append(triangle->normal().y());
            vector.append(triangle->normal().z());
            vector.append(static_cast<float>(index()));
        }
    }
}

GemRenderer::GemRenderData::GemRenderData() :
    m_allocatedGems(0)
  , m_allocatedAndUsedGems(0)
  , m_dataBuffer(0)
  , m_gems(new QList<GemDataInfo *>())
  , m_isInitialized(false)
  , m_lowestUnusedIndex(0)
  , m_vertexBuffer(nullptr)
{
}

GemRenderer::GemRenderData::~GemRenderData()
{
    delete m_vertexBuffer;

    for (auto gem : *m_gems) {
        delete gem;
    }
    delete m_gems;
}

void GemRenderer::GemRenderData::cleanup(QOpenGLFunctions &gl)
{
    if (!m_isInitialized) {
        return;
    }
    m_vertexBuffer->destroy();
    gl.glDeleteTextures(1, &m_dataBuffer);
}

void GemRenderer::GemRenderData::paint(QOpenGLFunctions &gl, QOpenGLShaderProgram &program)
{
    if (!m_isInitialized) {
        initialize(gl);
    }
    program.bind();
    program.setUniformValue("u_data", 7);
    program.setUniformValue("u_sceneExtent", m_sceneExtent);
    program.setUniformValue("u_maxNumberOfGems", static_cast<float>(m_allocatedGems));

    gl.glActiveTexture(GL_TEXTURE7);
    gl.glEnable(GL_TEXTURE_2D);
    gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
    m_vertexBuffer->bind();

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
    gl.glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (6 * sizeof(float)));
    gl.glDrawArrays(GL_TRIANGLES, 0, m_allocatedAndUsedGems * m_verticesPerGem);

    gl.glBindTexture(GL_TEXTURE_2D, 0);
    gl.glDisable(GL_TEXTURE_2D);
    m_vertexBuffer->release();
}

void GemRenderer::GemRenderData::initialize(QOpenGLFunctions &gl)
{
    m_isInitialized = true;
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vertexBuffer->create();

    gl.glGenTextures(1, &m_dataBuffer);
    gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void GemRenderer::GemRenderData::addOrUpdateGem(GemDataInfo *gem, QOpenGLFunctions &gl)
{
    if (!m_isInitialized) {
        initialize(gl);
    }
    if (!m_gems->contains(gem)) {
        addGem(gem, gl);
    } else {
        updateGem(gem, gl);
    }
}

void GemRenderer::GemRenderData::setVerticesPerGem(int numberOfVertices)
{
    m_verticesPerGem = numberOfVertices;
}

void GemRenderer::GemRenderData::setSceneExtent(float extent)
{
    m_sceneExtent = extent;
}

QPair<GLubyte, GLubyte> encodeIntoTwoGLubyte(float value, float min, float max)
{
    const int maxTwoByteValue = 256 * 256 - 1;
    float scaleDown = 1.f / (max - min);
    float valueScaledDown = (value - min) * scaleDown;
    unsigned int resultInt = static_cast<int>(valueScaledDown * maxTwoByteValue);
    resultInt = resultInt < 0 ? 0 : (resultInt > maxTwoByteValue ? maxTwoByteValue : resultInt);
    QPair<GLubyte, GLubyte> result;
    result.first = static_cast<GLubyte>(resultInt >> 8);
    result.second = static_cast<GLubyte>(resultInt);
    return result;
}

GLubyte asNormalizedGLubyte(float value, float maxExtent)
{
    //move value from [-maxExtent;maxExtent] to [0;1]
    float scaleDown = 1.f / (2.f * maxExtent);
    float valueZeroToOne = value * scaleDown + 0.5f;
    //move value from [0;1] to [0;255]
    return static_cast<GLubyte>(valueZeroToOne * 255);
}

void GemRenderer::GemRenderData::addGem(GemDataInfo *gem, QOpenGLFunctions &gl)
{
    if (m_allocatedAndUsedGems == m_allocatedGems) {
        m_allocatedGems += 256;
        m_vertexBuffer->bind();
        m_vertexBuffer->allocate(m_allocatedGems * m_verticesPerGem * 7 * sizeof(float));
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
        gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, m_allocatedGems, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        QVector<float> vertices;
        QVector<GLubyte> data;
        for (GemDataInfo *gem : *m_gems) {
            gem->appendVerticesWithIndexTo(vertices);
            GemData gemData = gem->data();
            QPair<GLubyte, GLubyte> encodedValueX = encodeIntoTwoGLubyte(gemData.position().x(), -m_sceneExtent, m_sceneExtent);
            QPair<GLubyte, GLubyte> encodedValueY = encodeIntoTwoGLubyte(gemData.position().y(), -m_sceneExtent, m_sceneExtent);
            QPair<GLubyte, GLubyte> encodedValueZ = encodeIntoTwoGLubyte(gemData.position().z(), -m_sceneExtent, m_sceneExtent);
            QPair<GLubyte, GLubyte> encodedValueS = encodeIntoTwoGLubyte(gemData.scale(), -m_sceneExtent, m_sceneExtent);

            data.append(encodedValueX.first);
            data.append(encodedValueY.first);
            data.append(encodedValueZ.first);
            data.append(encodedValueS.first);
            data.append(encodedValueX.second);
            data.append(encodedValueY.second);
            data.append(encodedValueZ.second);
            data.append(encodedValueS.second);
            data.append(asNormalizedGLubyte(gemData.rotation().x(), 1.f));
            data.append(asNormalizedGLubyte(gemData.rotation().y(), 1.f));
            data.append(asNormalizedGLubyte(gemData.rotation().z(), 1.f));
            data.append(asNormalizedGLubyte(gemData.rotation().scalar(), 1.f));
            data.append(static_cast<GLubyte>(gemData.color().x() * 255));
            data.append(static_cast<GLubyte>(gemData.color().y() * 255));
            data.append(static_cast<GLubyte>(gemData.color().z() * 255));
            data.append(255);   //padding
        }
        m_vertexBuffer->write(0, vertices.data(), vertices.size() * sizeof(float));
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4, m_allocatedAndUsedGems, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        m_vertexBuffer->release();
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
    }

    gem->setIndex(m_allocatedAndUsedGems++);
    m_gems->append(gem);

    QVector<float> verticesNew;
    gem->appendVerticesWithIndexTo(verticesNew);

    m_vertexBuffer->bind();
    m_vertexBuffer->write(gem->index() * m_verticesPerGem * 7 * sizeof(float), verticesNew.data(), verticesNew.size() * sizeof(float));
    m_vertexBuffer->release();

    updateGem(gem, gl);
}

void GemRenderer::GemRenderData::updateGem(GemDataInfo *gem, QOpenGLFunctions &gl)
{
    QVector<GLubyte> dataNew;
    GemData gemData = gem->data();

    QPair<GLubyte, GLubyte> encodedValueX = encodeIntoTwoGLubyte(gemData.position().x(), -m_sceneExtent, m_sceneExtent);
    QPair<GLubyte, GLubyte> encodedValueY = encodeIntoTwoGLubyte(gemData.position().y(), -m_sceneExtent, m_sceneExtent);
    QPair<GLubyte, GLubyte> encodedValueZ = encodeIntoTwoGLubyte(gemData.position().z(), -m_sceneExtent, m_sceneExtent);
    QPair<GLubyte, GLubyte> encodedValueS = encodeIntoTwoGLubyte(gemData.scale(), -m_sceneExtent, m_sceneExtent);

    dataNew.append(encodedValueX.first);
    dataNew.append(encodedValueY.first);
    dataNew.append(encodedValueZ.first);
    dataNew.append(encodedValueS.first);
    dataNew.append(encodedValueX.second);
    dataNew.append(encodedValueY.second);
    dataNew.append(encodedValueZ.second);
    dataNew.append(encodedValueS.second);
    dataNew.append(asNormalizedGLubyte(gemData.rotation().x(), 1.f));
    dataNew.append(asNormalizedGLubyte(gemData.rotation().y(), 1.f));
    dataNew.append(asNormalizedGLubyte(gemData.rotation().z(), 1.f));
    dataNew.append(asNormalizedGLubyte(gemData.rotation().scalar(), 1.f));
    dataNew.append(static_cast<GLubyte>(gemData.color().x() * 255));
    dataNew.append(static_cast<GLubyte>(gemData.color().y() * 255));
    dataNew.append(static_cast<GLubyte>(gemData.color().z() * 255));
    dataNew.append(255);   //padding

    gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
    gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, gem->index(), 4, 1, GL_RGBA, GL_UNSIGNED_BYTE, dataNew.data());
    gl.glBindTexture(GL_TEXTURE_2D, 0);
}
