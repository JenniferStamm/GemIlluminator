#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

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

    const QVector3D &initialRotation() const;
    void setInitialRotation(const QVector3D &initialRotation);

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    const QVector3D &rotation() const;
    void setRotation(const QVector3D &rotation);

protected:
    QVector3D *m_initialRotation;
    QVector3D *m_position;
    QVector3D *m_rotation;
};

#endif // GEOMETRYRENDERER_H
