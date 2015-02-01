#include "scenerenderer.h"

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

void SceneRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &gemProgram)
{
    for (auto& geometry : m_geometries) {
        geometry->paint(gl, viewProjection, gemProgram);
    }
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
