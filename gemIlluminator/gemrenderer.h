#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

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

        //packed draw related stuff
        void appendVerticesTo(QVector<float> &vector) const;

    protected:
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
    void paintGemsOptimizedWithTexture(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    void updateGemForTextureOptimization(AbstractGem *gem);

protected:
    bool m_isInitialized;
    QHash<AbstractGem *, GemDataInfo *> *m_gemMap;
    QHash<GemType, QList<QOpenGLBuffer *> *> *m_gemBuffers;
    bool m_isGemBufferUpdateRequired;
    bool m_isGemDataBufferInvalid;
    QList<GemDataInfo *> *m_newGems;
    QHash<GemType, GemRenderData *> *m_gemBuffersTex;
    float m_sceneExtent;
};

#endif // GEMRENDERERNEW_H
