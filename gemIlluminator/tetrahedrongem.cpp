#include "tetrahedrongem.h"

#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

TetrahedronGem::TetrahedronGem(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(3.f);
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    QVector<QVector3D> vertices;
    vertices.append(QVector3D( 1.f, -1.f, -1.f));
    vertices.append(QVector3D(-1.f, -1.f,  1.f));
    vertices.append(QVector3D( 1.f,  1.f,  1.f));
    vertices.append(QVector3D(-1.f,  1.f, -1.f));

    m_triangles->append(new Triangle(
                vertices.at(1),
                vertices.at(3),
                vertices.at(2),
                this));
    m_triangles->append(new Triangle(
                vertices.at(0),
                vertices.at(3),
                vertices.at(2),
                this));
    m_triangles->append(new Triangle(
                vertices.at(0),
                vertices.at(1),
                vertices.at(3),
                this));
    m_triangles->append(new Triangle(
                vertices.at(0),
                vertices.at(2),
                vertices.at(1),
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
