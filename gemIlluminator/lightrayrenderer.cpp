#include "lightrayrenderer.h"

LightRayRenderer::LightRayRenderer(QObject *parent) :
    QObject(parent)
  , m_isInitialized(false)
  , m_program(new QOpenGLProgram())
  , m_camera(nullptr)
{
}

virtual LightRayRenderer::~LightRayRenderer()
{
    delete m_program;
}

void LightRayRenderer::setCamera(const Camera & camera)
{
    delete m_camera;
    m_camera = new Camera(camera, this);
}

void LightRayRenderer::addLightRay(const LightRay & ray)
{

}

virtual void LightRayRenderer::paint(QOpenGLFunctions *gl)
{

}
