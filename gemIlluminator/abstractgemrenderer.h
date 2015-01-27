#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QObject>

class QMatrix4x4;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QVector3D;

class AbstractGemRenderer : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGemRenderer(QObject *parent = 0);
    virtual ~AbstractGemRenderer();

    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program) = 0;

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const QMatrix4x4 &model() const;
    void setModel(const QMatrix4x4 &model);

protected:
    QMatrix4x4 *m_model;
    QVector3D *m_color;
};

#endif // GEOMETRYRENDERER_H
