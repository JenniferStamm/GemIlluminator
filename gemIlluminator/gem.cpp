#include "gem.h"

#include "gemrenderer.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

Gem::Gem(QObject *parent) :
    AbstractGem(parent)
  , m_triangles()
  , m_vertices(new QVector<QVector3D>())
  , m_colors(new QVector<QVector3D>())
{
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    m_vertices->append(QVector3D(0.f, 0.5, 0.f));
    m_vertices->append(QVector3D(-0.5, -0.5, 0.5));
    m_vertices->append(QVector3D(0, -0.5, -0.5));
    m_vertices->append(QVector3D(0.5, -0.5, 0.5));

    m_colors->append(QVector3D(0.0, 0.0, 1.0));
    m_colors->append(QVector3D(1.0, 0.0, 0.0));
    m_colors->append(QVector3D(1.0, 1.0, 0.0));
    m_colors->append(QVector3D(0.0, 1.0, 1.0));

    m_triangles[0] = new Triangle(
                &m_vertices->at(1),
                &m_vertices->at(3),
                &m_vertices->at(2),
                &m_colors->at(0));
    m_triangles[1] = new Triangle(
                &m_vertices->at(0),
                &m_vertices->at(3),
                &m_vertices->at(2),
                &m_colors->at(1));
    m_triangles[2] = new Triangle(
                &m_vertices->at(0),
                &m_vertices->at(1),
                &m_vertices->at(3),
                &m_colors->at(2));
    m_triangles[3] = new Triangle(
                &m_vertices->at(0),
                &m_vertices->at(2),
                &m_vertices->at(1),
                &m_colors->at(3));
}

Gem::~Gem()
{
}

void Gem::synchronize()
{
    //renderer has to been created in correct thread
    if (!m_renderer){
        m_renderer = new GemRenderer(m_vertices, m_colors);
    }

    m_renderer->setInitialRotation(m_initialRotation);
    m_renderer->setPosition(m_position);
    m_renderer->setRotation(m_rotation);
}

void Gem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void Gem::update(int timeDifference)
{

}
