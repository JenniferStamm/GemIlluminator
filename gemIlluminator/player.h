#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class QVector3D;

class Camera;
class LightRay;

/**
 * @brief The Player class' only responsibilities are riding on lightrays and updating the camera.
 */
class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(qreal velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)

public:
    explicit Player(QObject *parent = 0);
    virtual ~Player();

    /**
     * @brief Moves the player along a ray.
     * @param ray The ray that is followed by ray.
     * @param timeDifferenceInMilliseconds The time elapsed since last update in order to calculate how far the player should move.
     */
    void moveOnRay(const LightRay &ray, int timeDifferenceInMilliseconds);
    /**
     * @brief Sest the player to ray.startPosition() and updates the camera accordingly.
     * @param ray
     * @seealso moveToEndPointOnRay();
     */
    void moveToStartPointOnRay(const LightRay &ray);
    /**
     * @brief Sets the player to ray.endPosition() and updates the camera accordingly.
     * @param ray
     * @seealso moveToEndPointOnRay();
     */
    void moveToEndPointOnRay(const LightRay &ray);

    /**
     * @brief The position of player in world coordinates
     * @return
     */
    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    /**
     * @brief Set view direction to provided value. For now this value should be the same like the direction of followed lightray.
     * @param viewDirection
     */
    void setViewDirection(const QVector3D &viewDirection);

signals:
    void cameraChanged();
    void velocityChanged();

public slots:
    /**
     * @brief The camera which is updated by player.
     * @return
     */
    Camera* camera() const;
    /**
     * @brief Sets the camera, that should be updated by player. The player does not take ownership of camera.
     * @param camera
     */
    void setCamera(Camera *camera);

    /**
     * @brief The velocity of player.
     * @return Returns the distance, that is passed in one second.
     */
    qreal velocity() const;
    /**
     * @brief Sets velocity of player
     * @param velocity
     */
    void setVelocity(qreal velocity);

protected:
    /**
     * @brief Updates the camera position for given point on ray
     * @param point The new position() of player on ray. This point has to lie on ray or it will lead to unexpected behaviour
     * @param ray The lightray the player is following.
     */
    void updateCameraForPointOnRay(const QVector3D &point, const LightRay & ray);

protected:
    qreal m_velocity;
    QVector3D *m_position;
    Camera *m_camera;
};

#endif // PLAYER_H
