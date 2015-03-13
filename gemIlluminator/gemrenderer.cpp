#include "gemrenderer.h"

#include <QDebug>
#include <QHash>
#include <QList>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLExtensions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QPair>
#include <QSurfaceFormat>
#include <QtGlobal>
#include <QVector>
#include <QVector3D>

#include "abstractgem.h"
#include "config.h"
#include "gemdata.h"
#include "triangle.h"

GemRenderer::GemRenderer() :
    m_isInitialized(false)
  , m_areFloatTexturesAvailable(false)
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
    for (auto renderData : m_gemBuffersTex->values()) {
        delete renderData;
    }
    delete m_gemBuffersTex;
}

void GemRenderer::cleanup(QOpenGLFunctions &gl)
{
    for (auto renderData : m_gemBuffersTex->values()) {
        renderData->cleanup(gl);
    }
}

void GemRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_isInitialized) {
        initialize(gl);
    }

    updateData(gl);

    program.enableAttributeArray(2);
    for (auto renderData : m_gemBuffersTex->values()) {
        renderData->paint(gl, program);
    }
}

void GemRenderer::setSceneExtent(float extent)
{
    m_sceneExtent = extent;
    m_isGemDataBufferInvalid = true;
}

void GemRenderer::updateGem(AbstractGem *gem)
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

void GemRenderer::initialize(QOpenGLFunctions &gl)
{
    if (m_isInitialized) {
        return;
    }
    QOpenGLContext *currentContext = QOpenGLContext::currentContext();
    m_areFloatTexturesAvailable = false;

#ifdef __ANDROID__
    m_areFloatTexturesAvailable = currentContext->hasExtension("OES_texture_float");
#else
    //if (!currentContext->isOpenGLES()) {
    //    QSurfaceFormat format = currentContext->format();
    //    if (format.majorVersion() > 3) {
    //        m_areFloatTexturesAvailable = true;
    //    } else if ((format.majorVersion() == 3) && (format.minorVersion() >= 2)) {
    //        m_areFloatTexturesAvailable = true;
    //    }
    //}
#endif
    m_isInitialized = true;
    //first data update is done while init because it is expected to be largest update and after init time measurment will be started (TODO)
    updateData(gl);

    for (auto renderData : m_gemBuffersTex->values()) {
        renderData->initialize(gl);
    }
}

void GemRenderer::updateData(QOpenGLFunctions &gl)
{
    if (!m_newGems->isEmpty()) {
        for (auto gem : *m_newGems) {
            GemRenderData *typeRenderData = m_gemBuffersTex->value(gem->data().type());
            if (!typeRenderData) {
                typeRenderData = new GemRenderData();
                typeRenderData->setVerticesPerGem(gem->numberOfVertices());
                typeRenderData->setSceneExtent(m_sceneExtent);
                typeRenderData->setFloatTexturesEnabled(m_areFloatTexturesAvailable);
                m_gemBuffersTex->insert(gem->data().type(), typeRenderData);
            }
            typeRenderData->addOrUpdateGem(gem, gl);
        }

        m_newGems->clear();
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
  , m_areFloatTexturesAvailable(false)
  , m_dataBuffer(0)
  , m_gems(new QList<GemDataInfo *>())
  , m_hasErrorOccured(false)
  , m_isInitialized(false)
  , m_lowestUnusedIndex(0)
  , m_maxTextureSize(64)    //OpenGL ES 2.0 specifies 64 as minimum value
  , m_texelPerGem(6)        //6 texels are needed if no float texture are available, we expect this
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
    if (m_hasErrorOccured) {
        return;
    }
    if (!m_isInitialized) {
        qDebug() << "GemRenderer::GemRenderData: Initalization should be done befor painting. It will done now, but measured times will be greater then they have to be.";
        initialize(gl);
    }

    float texWidth = (m_maxTextureSize / m_texelPerGem) * m_texelPerGem;
    float texHeight = m_allocatedGems / texWidth;

    program.bind();
    program.setUniformValue("u_data", 7);
    program.setUniformValue("u_sceneExtent", m_sceneExtent);
    program.setUniformValue("u_isFloatTextureAvailable", m_areFloatTexturesAvailable);
    program.setUniformValue("u_texHeight", texHeight);
    program.setUniformValue("u_texWidth", texWidth);
    program.setUniformValue("u_maxGemSize", Config::instance()->maxGemSize());
    program.setUniformValue("u_minGemSize", Config::instance()->minGemSize());

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
    if (m_isInitialized) {
        return;
    }
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

    gl.glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
    qDebug() << "tex:" << m_maxTextureSize;
}

void GemRenderer::GemRenderData::addOrUpdateGem(GemDataInfo *gem, QOpenGLFunctions &gl)
{
    if (m_hasErrorOccured) {
        return;
    }
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

void GemRenderer::GemRenderData::setFloatTexturesEnabled(bool enable)
{
    if (enable) {
        m_texelPerGem = 3;
    } else {
        m_texelPerGem = 6;
    }
    m_areFloatTexturesAvailable = enable;
}

void encode(float value, float min, float max, GLubyte &high, GLubyte &mid, GLubyte &low)
{
    const int maxValue = 256 * 256 * 256 - 1; //this value doesn't loose precission casting to float because highest int that can (and all lower ints) be stored in float is 2^24+1
    float scaleDown = static_cast<float>(maxValue) / (max - min);
    float valueScaledDown = (value - min) * scaleDown;
    unsigned int resultInt = std::round(valueScaledDown);
    resultInt = resultInt > maxValue ? maxValue : resultInt;
    high = static_cast<GLubyte>(resultInt >> 16);
    mid = static_cast<GLubyte>(resultInt >> 8);
    low = static_cast<GLubyte>(resultInt);
}

void GemRenderer::GemRenderData::appendAttributesToVector(GemDataInfo *gem, QVector<float> &vector)
{
    GemData gemData = gem->data();
    vector.append(gemData.position().x());
    vector.append(gemData.position().y());
    vector.append(gemData.position().z());
    vector.append(gemData.scale());
    vector.append(gemData.rotation().x());
    vector.append(gemData.rotation().y());
    vector.append(gemData.rotation().z());
    vector.append(gemData.rotation().scalar());
    vector.append(gemData.color().x());
    vector.append(gemData.color().y());
    vector.append(gemData.color().z());
    vector.append(1.f); //padding
}

void GemRenderer::GemRenderData::appendAttributesToVector(GemRenderer::GemDataInfo *gem, QVector<unsigned char> &vector)
{
    GemData gemData = gem->data();

    GLubyte highX, midX, lowX;
    GLubyte highY, midY, lowY;
    GLubyte highZ, midZ, lowZ;
    GLubyte highS, midS, lowS;
    //xyzs 3 texel per value because we want high precission and with 3 values per value we can use complete precission
    encode(gemData.position().x(), -m_sceneExtent, m_sceneExtent, highX, midX, lowX);
    encode(gemData.position().y(), -m_sceneExtent, m_sceneExtent, highY, midY, lowY);
    encode(gemData.position().z(), -m_sceneExtent, m_sceneExtent, highZ, midZ, lowZ);
    encode(gemData.scale(), Config::instance()->minGemSize(), Config::instance()->maxGemSize(), highS, midS, lowS);
    vector << highX << highY << highZ << highS;
    vector << midX << midY << midZ << midS;
    vector << lowX << lowY << lowZ << lowS;

    //rotation
    encode(gemData.rotation().x(), -1.f, 1.f, highX, midX, lowX);
    encode(gemData.rotation().y(), -1.f, 1.f, highY, midY, lowY);
    encode(gemData.rotation().z(), -1.f, 1.f, highZ, midZ, lowZ);
    encode(gemData.rotation().scalar(), -1.f, 1.f, highS, midS, lowS);
    vector << highX << highY << highZ << highS;
    vector << midX << midY << midZ << midS;

    //rgb 1 texel per value because there is no need for precission
    GLubyte colorHigh, colorMid, colorLow;
    encode(gemData.color().x(), 0.f, 1.f, colorHigh, colorMid, colorLow);
    vector << colorHigh;
    encode(gemData.color().y(), 0.f, 1.f, colorHigh, colorMid, colorLow);
    vector << colorHigh;
    encode(gemData.color().z(), 0.f, 1.f, colorHigh, colorMid, colorLow);
    vector << colorHigh;
    vector << 255; //padding
}

void GemRenderer::GemRenderData::addGem(GemDataInfo *gem, QOpenGLFunctions &gl)
{
    if (m_allocatedAndUsedGems == m_allocatedGems) {
        int gemsPerRow = m_maxTextureSize / m_texelPerGem;
        m_allocatedGems += gemsPerRow;
        int texWidth = gemsPerRow * m_texelPerGem;          //keep in mind: gemsPerRow is calculated using int division
        int texHeight = m_allocatedGems / gemsPerRow;
        if (texHeight > m_maxTextureSize) {
            qCritical() << "Can not store all required information for rendering with our implementation on GPU. Gems will not be rendered, choose less gems in options.";
            m_hasErrorOccured = true;
            return;
        }
        m_vertexBuffer->bind();
        m_vertexBuffer->allocate(m_allocatedGems * m_verticesPerGem * 7 * sizeof(float));
        QVector<float> vertices;
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
        if (m_areFloatTexturesAvailable) {
#ifdef __ANDROID__  //We support only android as mobile device, other OpenGL ES 2.0 devices should be checked too if needed
            gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_OES_texture_float, nullptr);
            QVector<float> data;
            for (GemDataInfo *gem : *m_gems) {
                gem->appendVerticesWithIndexTo(vertices);
                appendAttributesToVector(gem, data);
            }
            gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight - 1, GL_RGBA, GL_OES_texture_float, data.data());
#else   //We expect to have a OpenGL context instead of OpenGL ES, if it is build for no mobile devices
            gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texWidth, texHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
            QVector<float> data;
            for (GemDataInfo *gem : *m_gems) {
                gem->appendVerticesWithIndexTo(vertices);
                appendAttributesToVector(gem, data);
            }
            gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight - 1, GL_RGBA, GL_FLOAT, data.data());
#endif
        } else {
            gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            QVector<GLubyte> data;
            for (GemDataInfo *gem : *m_gems) {
                gem->appendVerticesWithIndexTo(vertices);
                appendAttributesToVector(gem, data);
            }
            gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight - 1, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        }
        m_vertexBuffer->write(0, vertices.data(), vertices.size() * sizeof(float));
        m_vertexBuffer->release();
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
    int texWidth = (m_maxTextureSize / m_texelPerGem) * m_texelPerGem;
    int xOffset = (gem->index() * m_texelPerGem) % texWidth;
    int yOffset = (gem->index() * m_texelPerGem) / texWidth;
    if (m_areFloatTexturesAvailable) {
        QVector<float> data;
        appendAttributesToVector(gem, data);
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
#ifdef __ANDROID__
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, m_texelPerGem, 1, GL_RGBA, GL_OES_texture_float, data.data());
#else
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, m_texelPerGem, 1, GL_RGBA, GL_FLOAT, data.data());
#endif
        gl.glBindTexture(GL_TEXTURE_2D, 0);
    } else {
        QVector<GLubyte> data;
        appendAttributesToVector(gem, data);
        gl.glBindTexture(GL_TEXTURE_2D, m_dataBuffer);
        gl.glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, m_texelPerGem, 1, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
        gl.glBindTexture(GL_TEXTURE_2D, 0);
    }
}
