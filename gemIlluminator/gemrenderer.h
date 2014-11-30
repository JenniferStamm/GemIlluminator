#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgeometryrenderer.h"

#include <QMatrix4x4>

class QOpenGLFunctions;
class QOpenGLBuffer;
class QOpenGLShaderProgram;

class GemRenderer : public AbstractGeometryRenderer
{
public:
    explicit GemRenderer(QObject *parent = 0);

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection) override;

protected:
    virtual void initialize();

private:
    QOpenGLBuffer *m_vertices;
    QOpenGLShaderProgram *m_program;
};

#endif // GEMRENDERER_H
