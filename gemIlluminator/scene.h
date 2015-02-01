#ifndef SCENE_H
#define SCENE_H

#include <QMap>
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
class SceneRenderer;
class Triangle;

enum class ShaderPrograms {
    GemProgram,
    LighRayProgram,
    EnvMapProgram
};

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractGem> geometries READ geometries NOTIFY geometriesChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(LightRay* rootLightRay READ rootLightRay WRITE setRootLightRay NOTIFY rootLightRayChanged)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<AbstractGem> geometries();

    Camera* camera() const;
    void setCamera(Camera *camera);

    SceneRenderer& sceneRenderer() const;


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

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

signals:
    void cubesChanged();
    void geometriesChanged();
    void rootLightRayChanged();

public slots:
    virtual void sync(int elapsedTime);
    virtual void cleanup();
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QMap<ShaderPrograms, QOpenGLShaderProgram*> shaderPrograms);
    void registerNavigation(Navigation *navigation);
    void rotateCurrentGem(const QQuaternion &quaternion);

protected:
    SceneRenderer *m_renderer;
    QList<AbstractGem*> m_gem;
    Camera *m_camera;
    Navigation *m_navigation;
    SceneBounds *m_bounds;
    AbstractGem *m_currentGem;
    LightRay *m_rootLightRay;
};

#endif // SCENE_H
