#ifndef GEMDATA_H
#define GEMDATA_H

#include <QObject>

template<typename T> class QList;
class QMatrix4x4;
class QQuaternion;
class QVector3D;

class AbstractGem;
enum class GemType;
class Triangle;

/**
 * @brief The GemData class stores all required information to describe a AbstractGem.
 * @detail The adventage of GemData is, that it is possible to assign, copy, compare and qHash() this class.
 * Therefore it is possible to store it in most Qt-containers.
 */
class GemData
{
public:
    /**
     * @brief Creates a new GemData with all values initialized to zero.
     */
    GemData();
    /**
     * @brief Copy constructor
     * @param ohterGemData GemData that should be copied.
     */
    GemData(const GemData &ohterGemData);

    ~GemData();

    GemData &operator=(const GemData &rhs);

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    /**
     * @brief Returns on demand the modelmatrix of current gemData
     * @return
     */
    const QMatrix4x4 &model() const;

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    const QQuaternion &rotation() const;
    void setRotation(const QQuaternion &rotation);

    float scale() const;
    void setScale(float scale);

    const QList<Triangle *> &triangles() const;
    void setTriangles(const QList<Triangle *> &triangles);

    GemType type() const;
    void setType(GemType type);

protected:
    void copyTriangles(const QList<Triangle *> &triangles);
    void calculateModelMatrix() const;

protected:
    QVector3D *m_color;
    mutable bool m_isModelInvalid;
    mutable QMatrix4x4 *m_model;
    QVector3D *m_position;
    QQuaternion *m_rotation;
    float m_scale;
    QList<Triangle *> *m_triangles;
    GemType m_type;
};

bool operator==(const GemData &lhs, const GemData &rhs);
bool operator!=(const GemData &lhs, const GemData &rhs);
uint qHash(const GemData &key, uint seed);

#endif // GEMDATA_H
