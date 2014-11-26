#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgeometryrenderer.h"

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

private:
    QOpenGLBuffer *m_vertices;
    QOpenGLBuffer *m_indices;
    QOpenGLShaderProgram *m_program;
};

#endif // GEMRENDERER_H
