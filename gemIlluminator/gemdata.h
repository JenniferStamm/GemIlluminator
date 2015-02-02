#ifndef GEMDATA_H
#define GEMDATA_H

#include <QObject>

template<typename T> class QList;
class QMatrix4x4;
class QQuaternion;
class QVector3D;

class AbstractGem;
class Triangle;

namespace Gem {
    enum class Type;
}

class GemData
{
public:
    GemData();
    GemData(const GemData &ohterGemData);
    GemData(const AbstractGem &gem);
    ~GemData();

    GemData &operator=(const GemData &rhs);

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    const QQuaternion &rotation() const;
    void setRotation(const QQuaternion &rotation);

    float scale() const;
    void setScale(float scale);

    const QList<Triangle *> &triangles() const;
    void setTriangles(const QList<Triangle *> &triangles);

    Gem::Type type() const;
    void setType(Gem::Type type);

protected:
    void copyTriangles(const QList<Triangle *> &triangles);

protected:
    QVector3D *m_color;
    QVector3D *m_position;
    QQuaternion *m_rotation;
    float m_scale;
    QList<Triangle *> *m_triangles;
    Gem::Type m_type;
};

bool operator==(const GemData &lhs, const GemData &rhs);
uint qHash(const GemData &key, uint seed);

#endif // GEMDATA_H
