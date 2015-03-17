#ifndef SCENEBOUNDS_H
#define SCENEBOUNDS_H

#include "abstractgem.h"

class QMatrix4x4;
class QQuaternion;

class LightRay;
class Scene;

/**
 * @brief The SceneBounds class is a special kind of gem describing the bounds of scene. The shape of the bounds is a cube, with a given extent in each direction.
 * @detail The main reason that the bounds are also a gem is easier collision detection. If we have bounds around our scene every ray emitted into scene will hit something.
 * Furthermore the collision with scene bounds can be processed in a way, that the player will lose if the player hits the bounds.
 */
class SceneBounds : public AbstractGem
{
    Q_OBJECT
public:
    /**
     * @brief Creates new SceneBounds. The extent of bounds is taken from config file.
     * @param parent
     */
    explicit SceneBounds(QObject *parent = 0);
    virtual ~SceneBounds();


    /**
     * @brief Override AbstractGem::setPosition() in order to forbid moving the bounds around.
     * @param position This parameter will be ignored.
     */
    void setPosition(const QVector3D &position) override;
    /**
     * @brief setRotation Override AbstractGem::setRotation() in order to forbid rotating the bounds.
     * @param rotation This parameter will be ignored.
     */
    void setRotation(const QQuaternion &rotation) override;

    /**
     * @brief Override AbstractGem::processRayIntersection() in order to ensure the player loses hitting the bounds.
     * @param ray The ray hitting the bounds.
     * @param scene The scene containing all lightrays.
     * @return Returns a QList containing only a GameLostRay, so the player will loose as soon as the player tries to move on returned ray.
     */
    QList<LightRay *> processRayIntersection(const LightRay &ray, Scene *scene) override;
};

#endif // SCENEBOUNDS_H
