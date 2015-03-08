#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

class QMatrix4x4;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QQuaternion;

class AbstractGemRenderer;
class GemData;
class LightRay;
class Triangle;
class Scene;

/**
 * @brief The GemType An enum describing current gem type. This enum is used for faster comparision of gems, because all gems of one type have same (objectspace) vertices.
 */
enum class GemType {
    Abstract,
    Cube,
    Tetrahedron};

/**
 * @brief Custom implementation of qHash. Providing hash calculation for GemType. In order to use GemType as key in QHash and QSet.
 * @param key Value the hash value is calculated for
 * @param seed
 * @return Returns hash value.
 */
uint qHash(GemType key, uint seed);

/**
 * @brief The AbstractGem class is our base class of all gems.
 * @detail As base class all required information of a gem are stored. Also usefull algorithms for collision detection are provided. Furthermore this class is supposed to be used within QML.
 */
class AbstractGem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D &position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(const QQuaternion &rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(const QVector3D &color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const GemData &data() const;

    const QMatrix4x4 &model() const;

    const QVector3D &position() const;
    virtual void setPosition(const QVector3D &position);

    qreal radius() const;

    const QQuaternion &rotation() const;
    virtual void setRotation(const QQuaternion &rotation);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    const QList<Triangle *> &triangles() const;
    GemType type() const;

    float boundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    float intersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    void rotate(const QQuaternion &quaternion);
    virtual QList<LightRay *> processRayIntersection(const LightRay &ray, Scene *scene);

public slots:
    /**
     * @brief Sets rotation of gem using euler angles. This method is mainly used to set initial rotation.
     * @param eulerRotation QVector3D containing the euler angles. The member of eulerVector contains corresponding rotation along axis (x component = rotation around x axis). The angle around axis is specified in degrees.
     */
    void setRotationFromEuler(const QVector3D &eulerRotation);

signals:
    void positionChanged();
    void rotationChanged();
    void scaleChanged();
    void colorChanged();

protected:
    int solveQuadricFormula(float a, float b, float c, float &x1, float &x2);

    /**
     * @brief Finds face of gem intersected by given ray. Ownership of returned face is transfered to caller.
     * @param ray Ray that might intersect gem
     * @param intersectedFace A pointer to intersected face is written into. Because this triangle is in worldspace and for performance reasons the ownership of face is transfered to caller. If ray does not intersect nullptr is written.
     * @param collisionPoint Optional parameter. If the given pointer is not nullptr the collisionpoint is written into.
     * @return Returns distance to collisionpoint. If no collission occured the value is maximum of float.
     */
    float faceIntersectedBy(const LightRay &ray, Triangle *&intersectedFace, QVector3D *collisionPoint = nullptr);

    /**
     * @brief Calculates triangle in world coordinates for given triangle. Therefor position, rotatition and scale of gem are used.
     * @param triangle Objectspace triangle for wich the coressponding wolrdspace triangle should be calculated.
     * @return Returns the Triangle in wolrd coordinates,
     */
    Triangle inWorldCoordinates(const Triangle &triangle);

protected:
    GemData *m_data;
    qreal m_radius;
};

#endif // GEOMETRY_H
