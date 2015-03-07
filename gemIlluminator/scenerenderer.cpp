#include "scenerenderer.h"

#include <QHash>
#include <QString>
#include <QDebug>

#include "abstractgem.h"
#include "config.h"
#include "gemrenderer.h"
#include "lightray.h"
#include "shaderprograms.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_gemRenderer(new GemRenderer())
  , m_isInitalized(false)
{
    m_gemRenderer->setSceneExtent(Config::instance()->axisRange());
}

SceneRenderer::~SceneRenderer()
{
    delete m_gemRenderer;
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
    m_rootLightRay->paint(gl, viewProjection, shaderProgram);
}

void SceneRenderer::synchronizeGeometries(QList<AbstractGem*> geometries)
{
    for (auto gem : geometries) {
        m_gemRenderer->updateGem(gem);
    }
    m_geometries = geometries;
}

LightRay* SceneRenderer::rootLightRay() const
{
    return m_rootLightRay;
}

void SceneRenderer::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
}
