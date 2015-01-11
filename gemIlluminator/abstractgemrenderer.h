#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

class QOpenGLFunctions;
class QOpenGLShaderProgram;

class AbstractGemRenderer : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGemRenderer(QObject *parent = 0);
    virtual ~AbstractGemRenderer();

    virtual void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program) = 0;

    QVector3D initialRotation() const;
    void setInitialRotation(const QVector3D &initialRotation);

    QVector3D position();
    void setPosition(QVector3D position);

    QVector3D rotation();
    void setRotation(QVector3D rotation);

protected:
    QVector3D m_initialRotation;
    QVector3D m_position;
    QVector3D m_rotation;
};

#endif // GEOMETRYRENDERER_H
