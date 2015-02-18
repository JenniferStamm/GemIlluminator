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

namespace{
    static const bool useOwnDrawCallForEachGem = false;
    static const bool useFewDrawCallsWithUniformArrays = false;
    static const bool useTextureBasedOtimization = true;
}

GemRenderer::GemRenderer() :
    m_isInitialized(false)
  , m_gemMap(new QHash<AbstractGem *, GemDataInfo *>())
  , m_maxUniformVectorSize(128) //128 is demanded by OpenGL ES 2.0 spec as minimum
  , m_gemBuffers(new QHash<GemType, QList<QOpenGLBuffer *> *>())
  , m_bufferDataLinkage(new QHash<QOpenGLBuffer *, QList<GemDataInfo *> *>())
  , m_isGemBufferUpdateRequired(false)
  , m_newGems(new QList<GemDataInfo *>())
  , m_sceneExtent(1.f)
  , m_isGemDataBufferInvalid(false)
  , m_gemBuffersTex(new QHash<GemType, GemRenderData *>())
{
}

GemRenderer::~GemRenderer()
{
    if (!useTextureBasedOtimization) {
        for (auto gem : m_gemMap->values()) {
            delete gem;
        }
    }
    delete m_gemMap;

    for (auto gemBufferList : m_gemBuffers->values()) {
        for (auto buffer : *gemBufferList) {
            delete buffer;
        }
        delete gemBufferList;
    }
    delete m_gemBuffers;

    for (auto list : m_bufferDataLinkage->values()) {
        delete list;
        //m_bufferDataLinkage links gpu with cpu data, therefore it owns neither buffers nor gem data
    }
    delete m_bufferDataLinkage;

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
        qDebug() << "Start rendering";
        initialize(gl);
    }
    if (useOwnDrawCallForEachGem) {
        paintAllGemsWithOwnDrawCall(gl, viewProjection, program);
    } else if (useFewDrawCallsWithUniformArrays) {
        paintGemsPackedUsingUniformArays(gl, viewProjection, program);
    } else if (useTextureBasedOtimization) {
        paintGemsOptimizedWithTexture(gl, viewProjection, program);
    }
}

void GemRenderer::setSceneExtent(float extent)
{
    m_sceneExtent = extent;
    m_isGemDataBufferInvalid = true;
}

void GemRenderer::updateGem(AbstractGem *gem)
{
   if (useOwnDrawCallForEachGem) {
       updateGemForOwnDrawCall(gem);
   } else if (useFewDrawCallsWithUniformArrays) {
       updateGemForUniformDrawCall(gem);
   } else if (useTextureBasedOtimization) {
       updateGemForTextureOptimization(gem);
   }
}

void GemRenderer::initialize(QOpenGLFunctions &gl)
{
    m_isInitialized = true;
    //gl.glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxUniformVectorSize);
}

void GemRenderer::paintAllGemsWithOwnDrawCall(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    program.bind();
    for (auto gemDataInfo : m_gemMap->values()) {
        QOpenGLBuffer &buffer = gemDataInfo->buffer();
        buffer.bind();

        program.setUniformValue("model", gemDataInfo->data().model());
        program.setUniformValue("normalMatrix", gemDataInfo->data().model().normalMatrix());
        program.setUniformValue("viewProjection", viewProjection);
        QMatrix4x4 mvp = viewProjection * gemDataInfo->data().model();
        program.setUniformValue("modelViewProjection", mvp);

        QVector3D color = gemDataInfo->data().color();
        program.setUniformValue("color", color);

        gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

        gl.glDrawArrays(GL_TRIANGLES, 0, gemDataInfo->numberOfVertices());

        buffer.release();
    }
}

void GemRenderer::paintGemsPackedUsingUniformArays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_newGems->isEmpty()) {
        //update required data structures
        for (auto gem : *m_newGems) {
            bool isNewBufferNeeded = false;
            QOpenGLBuffer *drawBuffer;
            QList<QOpenGLBuffer *> *typeBuffers = m_gemBuffers->value(gem->data().type());
            if (!typeBuffers) {
                typeBuffers = new QList<QOpenGLBuffer *>();
                m_gemBuffers->insert(gem->data().type(), typeBuffers);
                isNewBufferNeeded = true;
            } else {
                drawBuffer = typeBuffers->last();
                if (m_bufferDataLinkage->value(drawBuffer)->size() > m_maxUniformVectorSize) {
                    isNewBufferNeeded = true;
                }
            }
            if (isNewBufferNeeded) {
                drawBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
                drawBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
                drawBuffer->create();
                m_bufferDataLinkage->insert(drawBuffer,  new QList<GemDataInfo *>());
                typeBuffers->append(drawBuffer);
            }
            m_bufferDataLinkage->value(drawBuffer)->append(gem);
        }

        //update buffer data
        for (auto bufferList : m_gemBuffers->values()) {
            for (auto buffer : *bufferList) {
                float index = 0;
                buffer->bind();
                QVector<float> vertices;
                for (auto gemInfo : *(m_bufferDataLinkage->value(buffer))) {
                    //gemInfo->appendVerticesTo(vertices);
                    for (auto triangle : gemInfo->data().triangles()) {
                        for (auto vertex : triangle->vertices()) {
                            vertices.append(vertex.x());
                            vertices.append(vertex.y());
                            vertices.append(vertex.z());
                            vertices.append(triangle->normalizedNormal().x());
                            vertices.append(triangle->normalizedNormal().y());
                            vertices.append(triangle->normalizedNormal().z());
                            vertices.append(index);
                        }
                    }
                    ++index;
                }
                buffer->allocate(vertices.data(), vertices.size() * sizeof(float));
                buffer->release();
            }
        }
        m_newGems->clear();
    }

    program.bind();
    program.setUniformValue("u_viewProjection", viewProjection);
    program.enableAttributeArray(2);

    for (auto bufferList : m_gemBuffers->values()) {
        for (auto buffer : *bufferList) {
            QVector<float> xyzs;
            QVector<float> rotation;
            QVector<float> rgba;
            int verticesPerGem;

            QList<GemDataInfo *> *gemInfoList = m_bufferDataLinkage->value(buffer);
            for (auto gemInfo : *gemInfoList) {
                const GemData &data = gemInfo->data();
                verticesPerGem = gemInfo->numberOfVertices(); //this value is for all gems in one for loop the same
                xyzs.append(data.position().x());
                xyzs.append(data.position().y());
                xyzs.append(data.position().z());
                xyzs.append(data.scale());
                rotation.append(data.rotation().x());
                rotation.append(data.rotation().y());
                rotation.append(data.rotation().z());
                rotation.append(data.rotation().scalar());
                rgba.append(data.color().x());
                rgba.append(data.color().y());
                rgba.append(data.color().z());
                rgba.append(1.f);
            }

            program.setUniformValueArray("u_xyzs", xyzs.data(), xyzs.size(), 4);
            program.setUniformValueArray("u_rotation", rotation.data(), xyzs.size(), 4);
            program.setUniformValueArray("u_color", rgba.data(), xyzs.size(), 4);
            program.setUniformValue("u_verticesPerGem", verticesPerGem);

            buffer->bind();
            gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
            gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
            gl.glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (6 * sizeof(float)));
            gl.glDrawArrays(GL_TRIANGLES, 0, gemInfoList->size() * verticesPerGem);
            buffer->release();
        }
    }
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

void GemRenderer::updateGemForOwnDrawCall(AbstractGem *gem)
{
    if (m_gemMap->contains(gem)) {
        m_gemMap->value(gem)->setData(gem->data());
    } else {
        GemDataInfo *dataInfo = new GemDataInfo();
        dataInfo->setData(gem->data());
        m_gemMap->insert(gem, dataInfo);
    }
}

void GemRenderer::updateGemForUniformDrawCall(AbstractGem *gem)
{
    if (m_gemMap->contains(gem)) {
        m_gemMap->value(gem)->setData(gem->data());
    } else {
        GemDataInfo *dataInfo = new GemDataInfo();
        dataInfo->setData(gem->data());
        m_gemMap->insert(gem, dataInfo);
        m_newGems->append(dataInfo);
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
  , m_buffer(nullptr)
{
}

GemRenderer::GemDataInfo::~GemDataInfo()
{
    delete m_data;
    delete m_buffer;
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

QOpenGLBuffer &GemRenderer::GemDataInfo::buffer()
{
    if (!m_buffer) {
        m_buffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
        QVector<float> vertices;
        for (auto triangle : m_data->triangles()) {
            for (auto& vertex : triangle->vertices()) {
                vertices.append(vertex.x());
                vertices.append(vertex.y());
                vertices.append(vertex.z());
                vertices.append(triangle->normal().x());
                vertices.append(triangle->normal().y());
                vertices.append(triangle->normal().z());
            }
        }
        m_buffer->create();
        m_buffer->bind();
        m_buffer->allocate(vertices.data(), vertices.size() * sizeof(float));
        m_buffer->release();
    }
    return *m_buffer;
}

void GemRenderer::GemDataInfo::appendVerticesTo(QVector<float> &vector) const
{
    for (auto triangle : m_data->triangles()) {
        for (auto& vertex : triangle->vertices()) {
            vector.append(vertex.x());
            vector.append(vertex.y());
            vector.append(vertex.z());
            vector.append(triangle->normal().x());
            vector.append(triangle->normal().y());
            vector.append(triangle->normal().z());
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
            GemData gemData = gem->data();
            for (auto triangle : gemData.triangles()) {
                for (auto vertex : triangle->vertices()) {
                    vertices.append(vertex.x());
                    vertices.append(vertex.y());
                    vertices.append(vertex.z());
                    vertices.append(triangle->normalizedNormal().x());
                    vertices.append(triangle->normalizedNormal().y());
                    vertices.append(triangle->normalizedNormal().z());
                    vertices.append(gem->index());
                }
            }
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
        gl.glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        //m_vertexBuffer->write(0, vertices.data(), vertices.size() * sizeof(float));
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 4, m_allocatedAndUsedGems, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        m_vertexBuffer->release();
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
    }

    gem->setIndex(m_allocatedAndUsedGems++);
    m_gems->append(gem);

    QVector<float> verticesNew;
    for (auto triangle : gem->data().triangles()) {
        for (auto vertex : triangle->vertices()) {
            verticesNew.append(vertex.x());
            verticesNew.append(vertex.y());
            verticesNew.append(vertex.z());
            verticesNew.append(triangle->normalizedNormal().x());
            verticesNew.append(triangle->normalizedNormal().y());
            verticesNew.append(triangle->normalizedNormal().z());
            verticesNew.append(gem->index());
        }
    }
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
