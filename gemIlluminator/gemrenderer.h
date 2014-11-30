#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgeometryrenderer.h"

template <typename T> class QVector;
class QOpenGLFunctions;
class QOpenGLBuffer;
class QOpenGLShaderProgram;

class GemRenderer : public AbstractGeometryRenderer
{
public:
    explicit GemRenderer(QObject *parent = 0);

    void paint(QOpenGLFunctions *gl) override;

protected:
    virtual void initialize();
    virtual QVector<float> initializeVertexData(
            QVector<float> vector1,
            QVector<float> vector2,
            QVector<float> vector3,
            QVector<float> vector4,
            QVector<float> color1,
            QVector<float> color2,
            QVector<float> color3,
            QVector<float> color4);

private:
    QOpenGLBuffer *m_vertices;
    QOpenGLShaderProgram *m_program;
};

#endif // GEMRENDERER_H
