#include "cubegem.h"

#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

CubeGem::CubeGem(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(3.f);

    QVector<QVector3D> vertices;
    vertices.append(QVector3D(-1.0f, -1.0f, -1.0f)); // back bottom left
    vertices.append(QVector3D( 1.0f, -1.0f, -1.0f)); // back bottom right
    vertices.append(QVector3D(-1.0f,  1.0f, -1.0f)); // back top left
    vertices.append(QVector3D( 1.0f,  1.0f, -1.0f)); // back top right
    vertices.append(QVector3D(-1.0f, -1.0f,  1.0f)); // front bottom left
    vertices.append(QVector3D( 1.0f, -1.0f,  1.0f)); // front bottom right
    vertices.append(QVector3D(-1.0f,  1.0f,  1.0f)); // front top left
    vertices.append(QVector3D( 1.0f,  1.0f,  1.0f)); // front top right

    // back-face top-left triangle
    m_triangles->append(new Triangle(
                vertices.at(0),
                vertices.at(2),
                vertices.at(3),
                this));
    // back-face bottom-right triangle
    m_triangles->append(new Triangle(
                vertices.at(0),
                vertices.at(3),
                vertices.at(1),
                this));
    // right-face top-right triangle
    m_triangles->append(new Triangle(
                vertices.at(1),
                vertices.at(3),
                vertices.at(7),
                this));
    // right-face bottom-left triangle
    m_triangles->append(new Triangle(
                vertices.at(1),
                vertices.at(7),
                vertices.at(5),
                this));
    // front-face top-left triangle
    m_triangles->append(new Triangle(
                vertices.at(5),
                vertices.at(7),
                vertices.at(6),
                this));
    // front-face bottom-right triangle
    m_triangles->append(new Triangle(
                vertices.at(5),
                vertices.at(6),
                vertices.at(4),
                this));
    // left-face top-right triangle
    m_triangles->append(new Triangle(
                vertices.at(4),
                vertices.at(6),
                vertices.at(2),
                this));
    // left-face bottom-left triangle
    m_triangles->append(new Triangle(
                vertices.at(4),
                vertices.at(2),
                vertices.at(0),
                this));
    // top-face bottom-left triangle
    m_triangles->append(new Triangle(
                vertices.at(2),
                vertices.at(6),
                vertices.at(7),
                this));
    // top-face top-right triangle
    m_triangles->append(new Triangle(
                vertices.at(2),
                vertices.at(7),
                vertices.at(3),
                this));
    // bottom-face top-left triangle
    m_triangles->append(new Triangle(
                vertices.at(4),
                vertices.at(0),
                vertices.at(1),
                this));
    // bottom-face bottom-right triangle
    m_triangles->append(new Triangle(
                vertices.at(4),
                vertices.at(1),
                vertices.at(5),
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
