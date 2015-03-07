#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include "player.h"

#include <QObject>
#include <QVector3D>

template <typename T> class QList;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QVector3D;

class AbstractGem;
class LightRayData;
class LightRayRenderer;
class Scene;

class LightRay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D &startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(const QVector3D &endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(const QVector3D &direction READ direction)
    Q_PROPERTY(const QVector3D &normalizedDirection READ normalizedDirection)
    Q_PROPERTY(const QVector3D &color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(Player *player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(Scene *scene READ scene WRITE setScene NOTIFY sceneChanged)

public:
    explicit LightRay(QObject *parent = 0);
    virtual ~LightRay();

    virtual void update(int timeDifference);

    QVector3D normalizedOrthogonalVector() const;

signals:
    void colorChanged();
    void startPositionChanged();
    void endPositionChanged();
    void playerChanged();
    void sceneChanged();

public slots:
    const QVector3D &startPosition() const;
    void setStartPosition(const QVector3D &position);
    const QVector3D &endPosition() const;
    void setEndPosition(const QVector3D &position);
    const QVector3D &direction() const;
    const QVector3D &normalizedDirection() const;
    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    AbstractGem *collidingGem() const;
    void setCollidingGem(AbstractGem *gem);

    Player *player() const;
    void setPlayer(Player *attachedPlayer);

    Scene *scene() const;
    void setScene(Scene *owningScene);

    bool isStatic() const;
    void setStatic();

    LightRay *selectedSuccessor();
    void setSelectedSuccessor(LightRay *successor);

    const QList<LightRay *> &successors();

    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);

protected:
    bool isPlayerBeforeCollisionPoint();
    void calculateSuccessors();

protected:
    AbstractGem *m_collidingGem;
    LightRayData *m_data;
    QList<LightRay *> *m_successors;
    LightRay *m_selectedSuccessor;
    LightRayRenderer *m_renderer;
    bool m_isStatic;
    Player *m_player;
    Scene *m_scene;
};

#endif // LIGHTRAY_H
