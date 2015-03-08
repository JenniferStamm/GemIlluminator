#include "cubegem.h"

#include "gemdata.h"
#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

CubeGem::CubeGem(QObject *parent) :
    AbstractGem(parent)
{
    m_radius = sqrt(3.f);
    m_data->setType(GemType::Cube);

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
    QList<Triangle *> triangles;
    triangles.append(new Triangle(
                vertices.at(0),
                vertices.at(2),
                vertices.at(3)));
    // back-face bottom-right triangle
    triangles.append(new Triangle(
                vertices.at(0),
                vertices.at(3),
                vertices.at(1)));
    // right-face top-right triangle
    triangles.append(new Triangle(
                vertices.at(1),
                vertices.at(3),
                vertices.at(7)));
    // right-face bottom-left triangle
    triangles.append(new Triangle(
                vertices.at(1),
                vertices.at(7),
                vertices.at(5)));
    // front-face top-left triangle
    triangles.append(new Triangle(
                vertices.at(5),
                vertices.at(7),
                vertices.at(6)));
    // front-face bottom-right triangle
    triangles.append(new Triangle(
                vertices.at(5),
                vertices.at(6),
                vertices.at(4)));
    // left-face top-right triangle
    triangles.append(new Triangle(
                vertices.at(4),
                vertices.at(6),
                vertices.at(2)));
    // left-face bottom-left triangle
    triangles.append(new Triangle(
                vertices.at(4),
                vertices.at(2),
                vertices.at(0)));
    // top-face bottom-left triangle
    triangles.append(new Triangle(
                vertices.at(2),
                vertices.at(6),
                vertices.at(7)));
    // top-face top-right triangle
    triangles.append(new Triangle(
                vertices.at(2),
                vertices.at(7),
                vertices.at(3)));
    // bottom-face top-left triangle
    triangles.append(new Triangle(
                vertices.at(4),
                vertices.at(0),
                vertices.at(1)));
    // bottom-face bottom-right triangle
    triangles.append(new Triangle(
                vertices.at(4),
                vertices.at(1),
                vertices.at(5)));
    m_data->setTriangles(triangles);
}

CubeGem::~CubeGem()
{
}
