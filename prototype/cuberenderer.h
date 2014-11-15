#ifndef CUBERENDERER_H
#define CUBERENDERER_H

#include <QObject>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

class QOpenGLShaderProgram;

class CubeRenderer : public QObject
{
    Q_OBJECT
public:
    explicit CubeRenderer(QObject *parent = 0);
    virtual ~CubeRenderer();

    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    void paint(QOpenGLFunctions *gl);

protected:
    QVector3D m_position;
    QVector3D m_rotation;
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    QOpenGLBuffer m_vbo;
};

#endif // CUBERENDERER_H
