#include "scenerenderer.h"

#include <QMap>
#include <QString>
#include <QDebug>

#include "abstractgem.h"
#include "gemrenderer.h"
#include "lightray.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_gemRenderer(new GemRenderer())
  , m_sceneExtent(25.f)
{
    m_gemRenderer->setSceneExtent(m_sceneExtent);
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

void SceneRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QMap<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms)
{
    paintGems(gl, viewProjection, *shaderPrograms[ShaderPrograms::GemProgram]);
    paintLightRays(gl, viewProjection, *shaderPrograms[ShaderPrograms::LighRayProgram]);
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

void SceneRenderer::setSceneExtent(float extent)
{

}

LightRay* SceneRenderer::rootLightRay() const
{
    return m_rootLightRay;
}

void SceneRenderer::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
}
