#include "lightrayrenderer.h"

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include "camera.h"


LightRayRenderer::LightRayRenderer(QObject *parent) :
    QObject(parent)
  , m_isInitialized(false)
  , m_program(new QOpenGLShaderProgram(this))
  , m_camera(nullptr)
{
}

LightRayRenderer::~LightRayRenderer()
{
    delete m_program;
}

void LightRayRenderer::setCamera(Camera & camera)
{
    delete m_camera;
    m_camera = new Camera(camera, this);
}

void LightRayRenderer::addLightRay(const LightRay & ray)
{

}

void LightRayRenderer::paint(QOpenGLFunctions *gl)
{

}
