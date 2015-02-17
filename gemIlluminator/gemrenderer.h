#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

#include <QTime>

template<typename Key, typename T> class QHash;
template<typename T> class QList;
class QMatrix4x4;
class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QOpenGLTexture;
template<typename T> class QVector;

class AbstractGem;
class GemData;
enum class GemType;

class GemRenderer
{
    class GemDataInfo
    {
    public:
        GemDataInfo();
        ~GemDataInfo();

        const GemData &data() const;
        void setData(const GemData &data);

        int index() const;
        void setIndex(int index);

        int numberOfVertices();

        //single draw related stuff
        QOpenGLBuffer &buffer();

        //packed draw related stuff
        void appendVerticesTo(QVector<float> &vector) const;

    protected:
        //single draw related stuff
        QOpenGLBuffer *m_buffer;

        //packed draw related stuff
        int m_index;

        //stuff needed everytime
        GemData *m_data;
    };

    class GemRenderData
    {
    public:
        GemRenderData();
        ~GemRenderData();

        void cleanup(QOpenGLFunctions &gl);

        void paint(QOpenGLFunctions &gl, QOpenGLShaderProgram &program);
        void addOrUpdateGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        void setVerticesPerGem(int numberOfVertices);
        void setSceneExtent(float extent);

    protected:
        void addGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        void initialize(QOpenGLFunctions &gl);
        void updateGem(GemDataInfo *gem, QOpenGLFunctions &gl);

    protected:
        int m_allocatedGems;
        int m_allocatedAndUsedGems;
        unsigned int m_dataBuffer;
        QList<GemDataInfo *> *m_gems;
        bool m_isInitialized;
        unsigned int m_lowestUnusedIndex;
        QOpenGLBuffer *m_vertexBuffer;
        int m_verticesPerGem;
        float m_sceneExtent;
    };


public:
    GemRenderer();
    ~GemRenderer();

    void cleanup(QOpenGLFunctions &gl);
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);
    void setSceneExtent(float extent);
    void updateGem(AbstractGem *gem);

protected:
    void initialize(QOpenGLFunctions &gl);
    void paintAllGemsWithOwnDrawCall(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);
    void paintGemsPackedUsingUniformArays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);
    void paintGemsOptimizedWithTexture(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    void updateGemForOwnDrawCall(AbstractGem *gem);
    void updateGemForUniformDrawCall(AbstractGem *gem);
    void updateGemForTextureOptimization(AbstractGem *gem);

protected:
    bool m_isInitialized;
    int m_maxUniformVectorSize;
    QHash<AbstractGem *, GemDataInfo *> *m_gemMap;
    QHash<GemType, QList<QOpenGLBuffer *> *> *m_gemBuffers;
    QHash<QOpenGLBuffer *, QList<GemDataInfo *> *> *m_bufferDataLinkage;
    bool m_isGemBufferUpdateRequired;
    bool m_isGemDataBufferInvalid;
    QList<GemDataInfo *> *m_newGems;
    QHash<GemType, GemRenderData *> *m_gemBuffersTex;
    float m_sceneExtent;

    unsigned int m_counter;
    int m_elapsedTime;
    QTime m_initTime;
    QTime m_time;
};

#endif // GEMRENDERERNEW_H
