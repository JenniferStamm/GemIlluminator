#include "gemrenderer.h"

#include <QHash>
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QPair>
#include <QVector>
#include <QVector3D>

#include "abstractgem.h"
#include "gemdata.h"
#include "triangle.h"

namespace{
    static const bool useOwnDrawCallForEachGem = false;
    static const bool useFewDrawCallsWithUniformArrays = true;
}

GemRenderer::GemRenderer() :
    m_isInitialized(false)
  , m_gemMap(new QHash<AbstractGem *, GemDataInfo *>())
  , m_maxUniformVectorSize(128) //128 is demanded by OpenGL ES 2.0 spec as minimum
  , m_gemBuffers(new QHash<GemType, QList<QOpenGLBuffer *> *>())
  , m_bufferDataLinkage(new QHash<QOpenGLBuffer *, QList<GemDataInfo *> *>())
  , m_isGemBufferUpdateRequired(false)
  , m_newGems(new QList<GemDataInfo *>())
{
}

GemRenderer::~GemRenderer()
{
    for (auto gem : m_gemMap->values()) {
        delete gem;
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
}

void GemRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_isInitialized) {
        initialize(gl);
    }

    if (useOwnDrawCallForEachGem) {
        paintAllGemsWithOwnDrawCall(gl, viewProjection, program);
    } else if (useFewDrawCallsWithUniformArrays) {
        paintGemsPackedUsingUniformArays(gl, viewProjection, program);
    }
}

void GemRenderer::updateGem(AbstractGem *gem)
{
   if (useOwnDrawCallForEachGem) {
       updateGemForOwnDrawCall(gem);
   } else if (useFewDrawCallsWithUniformArrays) {
       updateGemForUniformDrawCall(gem);
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
            QOpenGLBuffer *drawBuffer;
            QList<QOpenGLBuffer *> *typeBuffers = m_gemBuffers->value(gem->data().type());
            bool isNewBufferNeeded = (typeBuffers == nullptr);
            if (typeBuffers) {
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
                    gemInfo->appendVerticesTo(vertices);
                    vertices.append(index++);
                }
                buffer->allocate(vertices.data(), vertices.size() * sizeof(float));
                buffer->release();
            }
        }
    }

    program.bind();
    program.setUniformValue("u_viewProjection", viewProjection);

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
