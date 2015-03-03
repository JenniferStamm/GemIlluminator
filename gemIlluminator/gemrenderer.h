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
        void appendVerticesWithIndexTo(QVector<float> &vector) const;

    protected:
        GemData *m_data;
        int m_index;
    };

    class GemRenderData
    {
    public:
        GemRenderData();
        ~GemRenderData();

        void cleanup(QOpenGLFunctions &gl);
        void initialize(QOpenGLFunctions &gl);

        void paint(QOpenGLFunctions &gl, QOpenGLShaderProgram &program);
        void addOrUpdateGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        void setVerticesPerGem(int numberOfVertices);
        void setSceneExtent(float extent);
        void setFloatTexturesEnabled(bool enable);
        void appendAttributesToVector(GemDataInfo *gem, QVector<float> &vector);
        void appendAttributesToVector(GemDataInfo *gem, QVector<unsigned char> &vector);

    protected:
        void addGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        void updateGem(GemDataInfo *gem, QOpenGLFunctions &gl);

    protected:
        int m_allocatedGems;
        int m_allocatedAndUsedGems;
        bool m_areFloatTexturesAvailable;
        unsigned int m_dataBuffer;
        QList<GemDataInfo *> *m_gems;
        bool m_hasErrorOccured;
        bool m_isInitialized;
        unsigned int m_lowestUnusedIndex;
        QOpenGLBuffer *m_vertexBuffer;
        int m_verticesPerGem;
        float m_sceneExtent;
        int m_maxTextureSize;
        int m_texelPerGem;
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
    void updateData(QOpenGLFunctions &gl);

protected:
    bool m_isInitialized;
    bool m_areFloatTexturesAvailable;
    QHash<AbstractGem *, GemDataInfo *> *m_gemMap;
    bool m_isGemBufferUpdateRequired;
    bool m_isGemDataBufferInvalid;
    QList<GemDataInfo *> *m_newGems;
    QHash<GemType, GemRenderData *> *m_gemBuffersTex;
    float m_sceneExtent;
};

#endif // GEMRENDERERNEW_H
