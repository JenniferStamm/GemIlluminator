#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>
#include <QQmlListProperty>

class AbstractGem;
class Camera;
class LightRay;
class Navigation;
class QTime;
class SceneBounds;
class SceneRenderer;
class Triangle;

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QQmlListProperty<AbstractGem> geometries READ geometries NOTIFY geometriesChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(LightRay* rootLightRay READ rootLightRay WRITE setRootLightRay)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<AbstractGem> geometries();

    qreal t() const;
    void setT(qreal t);

    bool isActive() const;
    void setActive(bool active);

    Camera* camera() const;
    void setCamera(Camera *camera);

    LightRay* rootLightRay() const;
    void setRootLightRay(LightRay *root);

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
     * @brief Finds intersected face of nearest gem intersected by given ray.
     * @param ray Ray send into scene to find gem face.
     * @param collisionPoint Optional parameter. The point of collision is written into. Only if no nullptr is returned this value is useable.
     * @return Returns the nearst intersected face of a gem. Returns never nullptr.
     */
    Triangle *findGemFaceIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr) const;

    void setCurrentGem(AbstractGem *currentGem);

signals:
    void cubesChanged();
    void tChanged();
    void geometriesChanged();
    void activeChanged();

public slots:
    virtual void sync();
    virtual void cleanup();
    void registerNavigation(Navigation *navigation);
    void rotateCurrentGem(const QQuaternion &quaternion);

protected:
    SceneRenderer *m_renderer;
    QList<AbstractGem*> m_gem;
    qreal m_t;
    QTime *m_time;
    bool m_active;
    Camera *m_camera;
    LightRay *m_rootLightRay;
    Navigation *m_navigation;
    SceneBounds *m_bounds;
    AbstractGem *m_currentGem;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // SCENE_H
