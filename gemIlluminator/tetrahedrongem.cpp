#include "tetrahedrongem.h"

#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

TetrahedronGem::TetrahedronGem(QObject *parent) :
    AbstractGem(parent)
  , m_vertices(new QVector<QVector3D>())
{
    m_radius = sqrt(3.f);
    m_scale = 0.3f;
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    m_vertices->append(QVector3D( 1.f, -1.f, -1.f));
    m_vertices->append(QVector3D(-1.f, -1.f,  1.f));
    m_vertices->append(QVector3D( 1.f,  1.f,  1.f));
    m_vertices->append(QVector3D(-1.f,  1.f, -1.f));

    m_triangles->append(new Triangle(
                m_vertices->at(1),
                m_vertices->at(3),
                m_vertices->at(2),
                this));
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(3),
                m_vertices->at(2),
                this));
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(1),
                m_vertices->at(3),
                this));
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(2),
                m_vertices->at(1),
                this));
}

TetrahedronGem::~TetrahedronGem()
{
}

void TetrahedronGem::synchronize()
{
    //renderer has to been created in correct thread
    if (!m_renderer){
        m_renderer = new GemRenderer(*m_triangles);
    }

    m_renderer->setInitialRotation(*m_initialRotation);
    m_renderer->setPosition(*m_position);
    m_renderer->setRotation(*m_rotation);
    m_renderer->setScale(m_scale);
    m_renderer->setColor(*m_color);
    m_renderer->setModel(model());
}

void TetrahedronGem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void TetrahedronGem::update(int timeDifference)
{

}
