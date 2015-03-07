#include "tetrahedrongem.h"

#include "gemdata.h"
#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

TetrahedronGem::TetrahedronGem(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(3.f);
    m_data->setType(GemType::Tetrahedron);
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    QVector<QVector3D> vertices;
    vertices.append(QVector3D( 1.f, -1.f, -1.f));
    vertices.append(QVector3D(-1.f, -1.f,  1.f));
    vertices.append(QVector3D( 1.f,  1.f,  1.f));
    vertices.append(QVector3D(-1.f,  1.f, -1.f));

    QList<Triangle *> triangles;

    triangles.append(new Triangle(
                vertices.at(1),
                vertices.at(2),
                vertices.at(3)));
    triangles.append(new Triangle(
                vertices.at(0),
                vertices.at(3),
                vertices.at(2)));
    triangles.append(new Triangle(
                vertices.at(0),
                vertices.at(1),
                vertices.at(3)));
    triangles.append(new Triangle(
                vertices.at(0),
                vertices.at(2),
                vertices.at(1)));
    m_data->setTriangles(triangles);
}

TetrahedronGem::~TetrahedronGem()
{
}
