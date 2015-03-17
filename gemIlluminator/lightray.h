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
class Scene;

/**
 * @brief The LightRay class describes the lightrays sent into Scene.
 * @detail Because LightRays are sent into Scene right after creation, they are more lines but rays.
 * Rays are organized as a tree, a ray owns all of its successors().
 * Most of the game logic is still done within LightRay::update().
 */
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

    /**
     * @brief Updates our game. The player will be moved.
     * @param timeDifference Time since last update in milliseconds.
     */
    virtual void update(int timeDifference);

    /**
     * @brief Calculates a normalized vector that is orthogonal to direction().
     * @return
     */
    QVector3D normalizedOrthogonalVector() const;

    /**
     * @brief calculateSuccessorColor calculates the successor color based on its normalized direction
     * The color values are approximately in the range between 0.1 and 0.8
     * @return The calculated color
     */
    QVector3D calculateColor();

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

    Player *player() const;
    void setPlayer(Player *attachedPlayer);

    Scene *scene() const;
    void setScene(Scene *owningScene);

    bool isStatic() const;
    void setStatic();

    /**
     * @brief Returns the ray the player should move on after reaching end of current ray. In case no successors exists they will be calculated using calculateSuccessors()
     * @return
     */
    LightRay *selectedSuccessor();
    void setSelectedSuccessor(LightRay *successor);

    const QList<LightRay *> &successors();

protected:
    bool isPlayerBeforeCollisionPoint();
    void calculateSuccessors();

    AbstractGem *collidingGem() const;
    void setCollidingGem(AbstractGem *gem);

protected:
    AbstractGem *m_collidingGem;
    LightRayData *m_data;
    QList<LightRay *> *m_successors;
    LightRay *m_selectedSuccessor;
    bool m_isStatic;
    Player *m_player;
    Scene *m_scene;
};

#endif // LIGHTRAY_H
