#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

template<typename T> class QList;
template<typename Key, typename T> class QHash;
class QMatrix4x4;
class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class AbstractGem;
class GemData;

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
    void initialize();
    void paintAllGemsWithOwnDrawCall(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

protected:
    bool m_isInitialized;
    QHash<AbstractGem *, GemDataInfo*> *m_gemMap;
};

#endif // GEMRENDERERNEW_H
