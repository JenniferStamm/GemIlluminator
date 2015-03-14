#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>
#include <QQmlListProperty>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QMatrix4x4;

class AbstractGem;
class Camera;
class LightRay;
class Navigation;
class SceneBounds;
class Triangle;

/**
 * @brief The Scene class provides access to geometry and collision detection methods.
 * Furthermore some game logic is implemented, so the scene holds the player, the gem inflicted by player and cameras.
 */
class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractGem> geometries READ geometriesQML NOTIFY geometriesChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(Camera* previewCamera READ previewCamera WRITE setPreviewCamera)
    Q_PROPERTY(LightRay* rootLightRay READ rootLightRay WRITE setRootLightRay NOTIFY rootLightRayChanged)

public:
    /**
     * @brief Creates a new scene without any further information.
     * @detail Before use scene you have to set geometries(), rootLightRay(), camera(), previewCamera() and registerNavigation()
     * @param parent
     */
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    /**
     * @brief Allow QML classes to read our gems.
     * @detail Initially it was planned to manipulate our gems from QML classes, but we did not get it to work.
     * Now we create it over a JSON-object and set it once, which works. All in all gem manipulation from QML
     * is magic and we do not know why and how it really works.
     * @return Returns a List of Gems which is supported by QML.
     */
    QQmlListProperty<AbstractGem> geometriesQML();
    QList<AbstractGem *> geometries();

    Camera* camera() const;
    void setCamera(Camera *camera);

    Camera* previewCamera() const;
    void setPreviewCamera(Camera *camera);

    /**
     * @brief Finds the nearest gem, that bounding sphere is intersected by given ray.
     * @param ray Ray send into scene to find gem.
     * @param collisionPoint Optional parameter. The point of collision is written into. Only if no nullptr is returned this value is useable.
     * @return Returns the nearst intersected gem. Returns never nullptr.
     */
    AbstractGem *findGemWithBoundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr) const;

    /**
     * @brief Finds the nearest gem with bounding sphere intersected by given ray.
     * @param ray Ray send into scene to find gem.
     * @param collisionPoint Optional parameter. The point of collision is written into.
     * @return Returns the nearst intersected gem. Returns never a nullptr;
     */
    AbstractGem *findGemIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr) const;

    /**
     * @brief Set the gem, that will be controlled by player.
     * @param currentGem The gem that will be controlled by player.
     */
    void setCurrentGem(AbstractGem *currentGem);

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

signals:
    void cubesChanged();
    void geometriesChanged();
    void rootLightRayChanged();
    void gameStarted();
    void gameLost();

public slots:
    virtual void update(int elapsedTime);

    void handleGameLost();
    void handleGameStarted();

    void registerNavigation(Navigation *navigation);
    void rotateCurrentGem(const QQuaternion &quaternion);

protected:
    SceneBounds *m_bounds;
    Camera *m_camera;
    Camera *m_previewCamera;
    AbstractGem *m_currentGem;
    QList<AbstractGem*> m_gems;
    Navigation *m_navigation;
    LightRay *m_rootLightRay;
};

#endif // SCENE_H
