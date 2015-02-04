#include "gemrenderer.h"

#include <QHash>
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QVector3D>

#include "abstractgem.h"
#include "gemdata.h"
#include "triangle.h"

GemRenderer::GemRenderer() :
    m_isInitialized(false)
  , m_gemMap(new QHash<AbstractGem *, GemDataInfo *>())
{
}

GemRenderer::~GemRenderer()
{
    for (auto gem : m_gemMap->values()) {
        delete gem;
    }
    delete m_gemMap;
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

void GemRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_isInitialized) {
        initialize();
    }

    paintAllGemsWithOwnDrawCall(gl, viewProjection, program);
}

void GemRenderer::updateGem(AbstractGem *gem)
{
    if (m_gemMap->contains(gem)) {
        m_gemMap->value(gem)->setData(gem->data());
    } else {
        GemDataInfo *dataInfo = new GemDataInfo();
        dataInfo->setData(gem->data());
        m_gemMap->insert(gem, dataInfo);
    }
}

void GemRenderer::initialize()
{
    m_isInitialized = true;
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
