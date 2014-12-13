#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

class AbstractGemRenderer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class AbstractGem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    virtual void synchronize() = 0;
    virtual void cleanup() = 0;

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program);

    QVector3D position();
    void setPosition(QVector3D position);

    QVector3D rotation();
    void setRotation(QVector3D rotation);

signals:
    void positionChanged();
    void rotationChanged();

protected:
    AbstractGemRenderer *m_renderer;
    QVector3D m_position;
    QVector3D m_rotation; /*!< Contains the euler angles */
};

#endif // GEOMETRY_H
