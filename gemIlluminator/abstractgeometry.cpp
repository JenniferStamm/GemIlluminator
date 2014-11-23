#include "abstractgeometry.h"

#include "abstractgeometryrenderer.h"

AbstractGeometry::AbstractGeometry(QObject *parent) :
    QObject(parent)
  , m_renderer(0)
{
}

AbstractGeometry::~AbstractGeometry()
{
    delete m_renderer;
}

void AbstractGeometry::paint(QOpenGLFunctions *gl)
{
    if (m_renderer) {
        m_renderer->paint(gl);
    }
}

void AbstractGeometry::synchronize()
{

}
