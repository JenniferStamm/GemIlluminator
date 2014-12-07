#include "gem.h"

#include "gemrenderer.h"

#include <QVector>
#include <QVector3D>

Gem::Gem(QObject *parent) :
    AbstractGeometry(parent)
,   m_vertices(new QVector<QVector3D>())
,   m_colors(new QVector<QVector3D>())
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

    m_renderer->setPosition(m_position);
    m_renderer->setRotation(m_rotation);
    m_renderer->setInitialRotation(m_initialRotation);
}

void Gem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

QVector3D Gem::initialRotation() const
{
    return m_initialRotation;
}

void Gem::setInitialRotation(const QVector3D &initialRotation)
{
    if (initialRotation == m_initialRotation) {
       return;
    }

    m_initialRotation = initialRotation;
    emit initialRotationChanged();
}
