#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>
#include <QQmlListProperty>

class AbstractGem;
class Camera;
class LightRay;
class Navigation;
class QTime;
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

    qreal t();
    void setT(qreal t);

    bool isActive();

    void setActive(bool active);

    Camera* camera();
    void setCamera(Camera *camera);

    LightRay* rootLightRay();
    void setRootLightRay(LightRay *root);

    AbstractGem *rayIntersection(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    AbstractGem *rayIntersectsTriangle(const LightRay &ray, QVector3D *collisionPoint = nullptr);

signals:
    void cubesChanged();
    void tChanged();
    void geometriesChanged();
    void activeChanged();

public slots:
    virtual void sync();
    virtual void cleanup();
    void registerNavigation(Navigation *navigation);

protected:
    SceneRenderer *m_renderer;
    QList<AbstractGem*> m_geometries;
    qreal m_t;
    QTime *m_time;
    bool m_active;
    Camera *m_camera;
    LightRay *m_rootLightRay;
    Navigation *m_navigation;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // SCENE_H
