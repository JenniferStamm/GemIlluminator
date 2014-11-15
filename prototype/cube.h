/*
 * Ein Cube ist aus QML-Sich nur eine Ansammlung von Daten, da es (glaube ich)
 * von den QQuickITems nicht beachtet wird bei Aktualisierungen usw, da es ein
 * QObject ist. Die Cubes werden nur auf dem Bildschrim repäsentiert im
 * Zusammenspiel mit dem Scenerenderer, der im richtigen Moment die paint()-Methode
 * aufruft.
*/

#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QVector3D>

class QOpenGLShaderProgram;

class CubeRenderer;

class Cube : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(qreal xRotation READ xRotation WRITE setXRotation NOTIFY xRotationChanged)
    Q_PROPERTY(qreal yRotation READ yRotation WRITE setYRotation NOTIFY yRotationChanged)
    Q_PROPERTY(qreal zRotation READ zRotation WRITE setZRotation NOTIFY zRotationChanged)

public:
    Cube(QObject *parent = 0);
    virtual ~Cube();

    void paint(QOpenGLFunctions *gl);
    void synchronize();

    qreal x();
    void setX(qreal x);

    qreal y();
    void setY(qreal y);

    qreal z();
    void setZ(qreal z);

    qreal xRotation();
    void setXRotation(qreal xRotation);

    qreal yRotation();
    void setYRotation(qreal yRotation);

    qreal zRotation();
    void setZRotation(qreal zRotation);

signals:
    void xChanged();
    void yChanged();
    void zChanged();
    void xRotationChanged();
    void yRotationChanged();
    void zRotationChanged();

protected:
    QVector3D m_position;
    QVector3D m_rotation;

    CubeRenderer *m_renderer;
};

#endif // CUBE_H
