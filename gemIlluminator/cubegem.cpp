#include "cubegem.h"

#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

CubeGem::CubeGem(QObject *parent) :
    AbstractGem(parent)
  , m_vertices(new QVector<QVector3D>())
{
    m_radius = sqrt(3.f);

    m_vertices->append(QVector3D(-1.0f, -1.0f, -1.0f)); // front bottom left
    m_vertices->append(QVector3D( 1.0f, -1.0f, -1.0f)); // front bottom right
    m_vertices->append(QVector3D(-1.0f,  1.0f, -1.0f)); // front top left
    m_vertices->append(QVector3D( 1.0f,  1.0f, -1.0f)); // front top right
    m_vertices->append(QVector3D(-1.0f, -1.0f,  1.0f)); // back bottom left
    m_vertices->append(QVector3D( 1.0f, -1.0f,  1.0f)); // back bottom right
    m_vertices->append(QVector3D(-1.0f,  1.0f,  1.0f)); // back top left
    m_vertices->append(QVector3D( 1.0f,  1.0f,  1.0f)); // back top right

    // front-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(3),
                m_vertices->at(2),
                this));
    // front-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(1),
                m_vertices->at(3),
                this));
    // right-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(1),
                m_vertices->at(7),
                m_vertices->at(3),
                this));
    // right-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(1),
                m_vertices->at(5),
                m_vertices->at(7),
                this));
    // back-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(5),
                m_vertices->at(6),
                m_vertices->at(7),
                this));
    // back-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(5),
                m_vertices->at(4),
                m_vertices->at(6),
                this));
    // left-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(2),
                m_vertices->at(6),
                this));
    // left-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(0),
                m_vertices->at(2),
                this));
    // top-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(2),
                m_vertices->at(7),
                m_vertices->at(6),
                this));
    // top-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(2),
                m_vertices->at(3),
                m_vertices->at(7),
                this));
    // bottom-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(1),
                m_vertices->at(0),
                this));
    // bottom-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(5),
                m_vertices->at(1),
                this));
}

CubeGem::~CubeGem()
{
}

void CubeGem::synchronize()
{
    //renderer has to been created in correct thread
    if (!m_renderer){
        m_renderer = new GemRenderer(*m_triangles);
    }

    m_renderer->setColor(*m_color);
    m_renderer->setModel(model());
}

void CubeGem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void CubeGem::update(int timeDifference)
{

}
