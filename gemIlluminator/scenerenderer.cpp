#include "scenerenderer.h"

#include <QMap>
#include <QString>
#include <QDebug>

#include "abstractgem.h"
#include "lightray.h"

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QMap<ShaderPrograms, QOpenGLShaderProgram*> shaderPrograms)
{
    paintGems(gl, viewProjection, *shaderPrograms[ShaderPrograms::GemProgram]);
    paintLightRays(gl, viewProjection, *shaderPrograms[ShaderPrograms::LighRayProgram]);
}

void SceneRenderer::paintGems(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram& shaderProgram)
{
    for (auto& geometry : m_geometries) {
        geometry->paint(gl, viewProjection, shaderProgram);
    }
}

void SceneRenderer::paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram& shaderProgram)
{
    m_rootLightRay->paint(gl);
}

void SceneRenderer::setGeometries(QList<AbstractGem*> geometries)
{
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
