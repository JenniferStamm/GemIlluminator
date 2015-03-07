#include "scenerenderer.h"

#include <QHash>
#include <QString>
#include <QDebug>

#include "abstractgem.h"
#include "config.h"
#include "gemrenderer.h"
#include "lightray.h"
#include "lightrayrenderer.h"
#include "shaderprograms.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_gemRenderer(new GemRenderer())
  , m_lightRayRenderer(new LightRayRenderer())
  , m_isInitalized(false)
{
    m_gemRenderer->setSceneExtent(Config::instance()->axisRange());
}

SceneRenderer::~SceneRenderer()
{
    delete m_gemRenderer;
    delete m_lightRayRenderer;
}

void SceneRenderer::cleanup(QOpenGLFunctions &gl)
{
    if (m_gemRenderer) {
        m_gemRenderer->cleanup(gl);
    }
}

void SceneRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms)
{
    if (!m_isInitalized) {
        initalize(gl);
    }
    paintGems(gl, viewProjection, *shaderPrograms.value(ShaderPrograms::GemProgram));
    paintLightRays(gl, viewProjection, *shaderPrograms.value(ShaderPrograms::LighRayProgram));
}

void SceneRenderer::initalize(QOpenGLFunctions &gl)
{
    m_gemRenderer->initialize(gl);
    m_isInitalized = true;
    emit initalizationDone();
}

void SceneRenderer::paintGems(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram& shaderProgram)
{
    m_gemRenderer->paint(gl, viewProjection, shaderProgram);
}

void SceneRenderer::paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram)
{
    m_lightRayRenderer->paint(gl, viewProjection, shaderProgram);
}

void SceneRenderer::synchronizeGeometries(QList<AbstractGem*> geometries)
{
    for (auto gem : geometries) {
        m_gemRenderer->updateGem(gem);
    }
}

void SceneRenderer::synchronizeLightRays(LightRay *rootLightRay)
{
    m_lightRayRenderer->resetDynamicRays();
    auto rays = QList<LightRay *>() << rootLightRay;
    int counter = 0;
    while (counter < rays.count()) {
        m_lightRayRenderer->addLightRay(*rays.at(counter));
        rays.append(rays.at(counter)->successors());
        ++counter;
    }
}
