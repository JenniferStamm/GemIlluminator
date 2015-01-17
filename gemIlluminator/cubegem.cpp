#include "cubegem.h"

#include "gemrenderer.h"
#include "lightray.h"
#include "triangle.h"

#include <QVector>
#include <QVector3D>

CubeGem::CubeGem(QObject *parent) :
    AbstractGem(parent)
  , m_triangles(new QVector<Triangle *>())
  , m_vertices(new QVector<QVector3D>())
  , m_colors(new QVector<QVector3D>())
{
    m_vertices->append(QVector3D(-0.5f, -0.5f, -0.5f)); // front bottom left
    m_vertices->append(QVector3D(0.5f, -0.5f, -0.5f));  // front bottom right
    m_vertices->append(QVector3D(-0.5f, 0.5f, -0.5f));  // front top left
    m_vertices->append(QVector3D(0.5f, 0.5f, -0.5f));   // front top right
    m_vertices->append(QVector3D(-0.5f, -0.5f, 0.5f));  // back bottom left
    m_vertices->append(QVector3D(0.5f, -0.5f, 0.5f));   // back bottom right
    m_vertices->append(QVector3D(-0.5f, 0.5f, 0.5f));   // back top left
    m_vertices->append(QVector3D(0.5f, 0.5f, 0.5f));    // back top right

    m_colors->append(QVector3D(0.0f, 0.0f, 1.0f));
    m_colors->append(QVector3D(0.0f, 1.0f, 0.0f));
    m_colors->append(QVector3D(0.0f, 1.0f, 1.0f));
    m_colors->append(QVector3D(1.0f, 0.0f, 0.0f));
    m_colors->append(QVector3D(1.0f, 0.0f, 1.0f));
    m_colors->append(QVector3D(1.0f, 1.0f, 0.0f));

    // front-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(3),
                m_vertices->at(2),
                m_colors->at(0)));
    // front-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(0),
                m_vertices->at(1),
                m_vertices->at(3),
                m_colors->at(0)));
    // right-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(1),
                m_vertices->at(7),
                m_vertices->at(3),
                m_colors->at(1)));
    // right-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(1),
                m_vertices->at(5),
                m_vertices->at(7),
                m_colors->at(1)));
    // back-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(5),
                m_vertices->at(6),
                m_vertices->at(7),
                m_colors->at(2)));
    // back-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(5),
                m_vertices->at(4),
                m_vertices->at(6),
                m_colors->at(2)));
    // left-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(2),
                m_vertices->at(6),
                m_colors->at(3)));
    // left-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(0),
                m_vertices->at(2),
                m_colors->at(3)));
    // top-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(2),
                m_vertices->at(7),
                m_vertices->at(6),
                m_colors->at(4)));
    // top-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(2),
                m_vertices->at(3),
                m_vertices->at(7),
                m_colors->at(4)));
    // bottom-face top-left triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(1),
                m_vertices->at(0),
                m_colors->at(5)));
    // bottom-face bottom-right triangle
    m_triangles->append(new Triangle(
                m_vertices->at(4),
                m_vertices->at(5),
                m_vertices->at(1),
                m_colors->at(5)));
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

    m_renderer->setInitialRotation(*m_initialRotation);
    m_renderer->setPosition(*m_position);
    m_renderer->setRotation(*m_rotation);
}

void CubeGem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

void CubeGem::update(int timeDifference)
{

}

float CubeGem::rayIntersect(const LightRay &ray, int &triangleIndex, QVector3D *collisionPoint)
{
    const float maxFloat = std::numeric_limits<float>::max();
    const QVector3D noCollisionPoint(maxFloat, maxFloat, maxFloat);

    // Calculate collision according to
    // http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    // http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    const float epsilon = 0.000001f;
    float tPrevious = maxFloat;
    float t, u, v;
    QVector3D edge1, edge2, tvec, pvec, qvec;
    float det, invDet;

    for (int i = 0; i < 4; i++) {
        Triangle *triangle = m_triangles->at(1);
        //QVector3D normal = triangle->normalizedNormal();

        edge1 = triangle->b() - triangle->a();
        edge2 = triangle->c() - triangle->a();

        // Calculate determinant
        pvec = QVector3D::crossProduct(ray.direction(), edge2);
        det = QVector3D::dotProduct(edge1, pvec);

        // Non-culling branch
        if (-epsilon < det && det < epsilon) {
            *collisionPoint = noCollisionPoint;
        } else {
            invDet = 1.f / det;

            // Calculate distance from vert0 to ray origin
            tvec = ray.startPosition() - triangle->a();

            // Calculate u and test bounds
            u = QVector3D::dotProduct(tvec, pvec) * invDet;
            if (u < 0.f || u > 1.f) {
                *collisionPoint = noCollisionPoint;
            } else {
                // Prepare to test v
                qvec = QVector3D::crossProduct(tvec, edge1);

                // Calculate v and test bounds
                v = QVector3D::dotProduct(ray.direction(), qvec) * invDet;
                if ( v < 0.f || u + v > 1.0) {
                    *collisionPoint = noCollisionPoint;
                } else {
                    // Calculate t, ray intersects triangle
                    t = QVector3D::dotProduct(edge2, qvec) * invDet;
                    if (t < tPrevious && t > 0.0 + epsilon) {
                        tPrevious = t;
                        triangleIndex = i;
                        *collisionPoint = ray.startPosition() + t * ray.direction();
                    }
                }

            }
        }
    }
    return tPrevious;
}
