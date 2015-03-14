#ifndef TRIANGLE_H
#define TRIANGLE_H

template<typename T> class QList;
class QVector3D;

/**
 * @brief The Triangle class represents a triangle in three dimensional space.
 * @details Mostly this is a data class storing the vertices and is easy copiable. Also some helper functions are implemented.
 */
class Triangle
{
public:
    /**
     * @brief Creates a new degenerated Triangle, with all points in (0, 0, 0).
     */
    Triangle();

    /**
     * @brief Creates a new Triangle, with specified points. It is expected, that points a, b and c are ordered counter clock wise.
     * @param a First vertex.
     * @param b Second vertex.
     * @param c Third vertex.
     */
    Triangle(const QVector3D &a,
             const QVector3D &b,
             const QVector3D &c);

    /**
     * @brief Creates a new Triangle with data copied from given triangle.
     * @param triangle The Triangle, that will be copied.
     */
    Triangle(const Triangle &triangle);

    ~Triangle();

    Triangle &operator=(const Triangle &triangle);

    const QVector3D &a() const;
    void setA(const QVector3D &a);

    const QVector3D &b() const;
    void setB(const QVector3D &b);

    const QVector3D &c() const;
    void setC(const QVector3D &c);

    /**
     * @brief Returns normal of triangle. The vertices a(), b() and c() ordered counter clockwise are expected to describe outer side of triangle.
     * @return
     */
    const QVector3D &normal() const;

    /**
     * @brief Convienence method. All vertices are returned in QList
     * @return QList of vertices. Containing a(), b() and c() in this order.
     */
    QList<QVector3D> vertices() const;

    /**
     * @brief Reflects incoming ray at outer side of triangle.
     * @param incidentVector The vector that will be reflected.
     * @return Reflected vector.
     */
    QVector3D reflect(const QVector3D &incidentVector) const;

protected:
    void calculateNormal() const;

protected:
    QVector3D *m_a;
    QVector3D *m_b;
    QVector3D *m_c;
    mutable QVector3D *m_normal;
};

#endif // TRIANGLE_H
