#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

template<typename Key, typename T> class QHash;
template<typename T> class QList;
class QMatrix4x4;
class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
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

public:
    GemRenderer();
    ~GemRenderer();

    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);
    void updateGem(AbstractGem *gem);

protected:
    void initialize(QOpenGLFunctions &gl);
    void paintAllGemsWithOwnDrawCall(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);
    void paintGemsPackedUsingUniformArays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    void updateGemForOwnDrawCall(AbstractGem *gem);
    void updateGemForUniformDrawCall(AbstractGem *gem);

protected:
    bool m_isInitialized;
    int m_maxUniformVectorSize;
    QHash<AbstractGem *, GemDataInfo *> *m_gemMap;
    QHash<GemType, QList<QOpenGLBuffer *> *> *m_gemBuffers;
    QHash<QOpenGLBuffer *, QList<GemDataInfo *> *> *m_bufferDataLinkage;
    bool *m_isGemBufferUpdateRequired;
    QList<GemDataInfo *> *m_newGems;
};

#endif // GEMRENDERERNEW_H
