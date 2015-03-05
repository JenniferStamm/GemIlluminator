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
class LightRayRenderer;
class Navigation;
class SceneBounds;
class SceneRenderer;
class Triangle;
enum class ShaderPrograms;

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractGem> geometries READ geometries NOTIFY geometriesChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera)
    Q_PROPERTY(Camera* previewCamera READ previewCamera WRITE setPreviewCamera)
    Q_PROPERTY(LightRay* rootLightRay READ rootLightRay WRITE setRootLightRay NOTIFY rootLightRayChanged)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<AbstractGem> geometries();

    Camera* camera() const;
    void setCamera(Camera *camera);

    Camera* previewCamera() const;
    void setPreviewCamera(Camera *camera);

    SceneRenderer& sceneRenderer() const;

    void paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QMap<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms);

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

    void setCurrentGem(AbstractGem *currentGem);

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

    bool isPlayerAlive() const;

signals:
    void cubesChanged();
    void geometriesChanged();
    void rootLightRayChanged();
    void gameStarted();
    void gameLost();

public slots:
    virtual void sync(int elapsedTime);
    virtual void cleanupGL(QOpenGLFunctions &gl);
    void handleGameLost();
    void handleGameStarted();
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QMap<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms);
    void registerNavigation(Navigation *navigation);
    void rotateCurrentGem(const QQuaternion &quaternion);

protected:
    SceneBounds *m_bounds;
    Camera *m_camera;
    Camera *m_previewCamera;
    AbstractGem *m_currentGem;
    QList<AbstractGem*> m_gem;
    LightRayRenderer *m_lightRayRenderer;
    Navigation *m_navigation;
    SceneRenderer *m_renderer;
    LightRay *m_rootLightRay;
};

#endif // SCENE_H
