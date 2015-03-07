#include "scene.h"

#include <cassert>
#include <limits>

#include <QQuickWindow>
#include <QVector3D>

#include "abstractgem.h"
#include "camera.h"
#include "lightray.h"
#include "lightrayrenderer.h"
#include "navigation.h"
#include "scenebounds.h"
#include "scenerenderer.h"
#include "shaderprograms.h"
#include "triangle.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_bounds(new SceneBounds())
  , m_camera(nullptr)
  , m_previewCamera(nullptr)
  , m_currentGem(m_bounds)
  , m_navigation(nullptr)
  , m_renderer(nullptr)
  , m_rootLightRay(nullptr)
{
}

Scene::~Scene()
{
    delete m_bounds;
    delete m_renderer;
}

void Scene::sync(int elapsedTime)
{
    m_rootLightRay->update(elapsedTime);

    if (!m_renderer) {
        m_renderer = new SceneRenderer();
        connect(m_renderer, &SceneRenderer::initalizationDone, this, &Scene::handleGameStarted);
    }

    m_renderer->synchronizeGeometries(m_gem);
    m_renderer->synchronizeLightRays(m_rootLightRay);
}

void Scene::cleanupGL(QOpenGLFunctions &gl)
{
    if (m_renderer) {
        m_renderer->cleanup(gl);
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void Scene::handleGameLost()
{
    emit gameLost();
}

void Scene::handleGameStarted()
{
    emit gameStarted();
}

void Scene::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms)
{
    m_renderer->paint(gl, viewProjection, shaderPrograms);
}

QQmlListProperty<AbstractGem> Scene::geometries()
{
    return QQmlListProperty<AbstractGem>(this, m_gem);
}

void Scene::registerNavigation(Navigation *navigation)
{
    if (m_navigation) {
        disconnect(m_navigation, &Navigation::worldSpaceRotationChanged, this, &Scene::rotateCurrentGem);
    }
    m_navigation = navigation;
    m_navigation->setCamera(m_camera);
    connect(m_navigation, &Navigation::worldSpaceRotationChanged, this, &Scene::rotateCurrentGem);
}

void Scene::rotateCurrentGem(const QQuaternion &quaternion)
{
    m_currentGem->rotate(quaternion);
}

Camera* Scene::camera() const
{
    return m_camera;
}

void Scene::setCamera(Camera* camera)
{
    m_camera = camera;
}

Camera* Scene::previewCamera() const
{
    return m_previewCamera;
}

void Scene::setPreviewCamera(Camera* camera)
{
    m_previewCamera = camera;
}

LightRay* Scene::rootLightRay() const
{
    return m_rootLightRay;
}

void Scene::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
}

SceneRenderer& Scene::sceneRenderer() const
{
    assert(m_renderer);
    return *m_renderer;
}

void Scene::paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms)
{
    m_renderer->paintLightRays(gl, viewProjection, *shaderPrograms[ShaderPrograms::LighRayProgram]);
}

AbstractGem *Scene::findGemIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    AbstractGem *result = m_bounds;
    float distance = m_bounds->intersectedBy(ray, collisionPoint);
    for( auto& gem : m_gem ){
        QVector3D temp;
        float collisionDistance = gem->intersectedBy(ray, &temp);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = temp;
            }
            result = gem;
        }
    }
    assert(result);
    return result;
}

AbstractGem *Scene::findGemWithBoundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    AbstractGem *result = m_bounds;
    float distance = m_bounds->boundingSphereIntersectedBy(ray, collisionPoint);
    for( auto& gem : m_gem ){
        QVector3D temp;
        float collisionDistance = gem->boundingSphereIntersectedBy(ray, &temp);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = temp;
            }
            result = gem;
        }
    }
    assert(result);
    return result;
}

void Scene::setCurrentGem(AbstractGem *currentGem)
{
    if (currentGem == nullptr) {
        return;
    }
    m_currentGem = currentGem;
}
