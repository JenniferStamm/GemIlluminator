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
 * @brief An enum describing current gem type. This enum is used for faster comparision of gems, because all gems of one type have same (objectspace) vertices.
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
 * @brief The AbstractGem class is the base class of all gems.
 * @detail As base class all required information of a gem are stored. Also, useful algorithms for collision detection are provided. Furthermore, this class is supposed to be used within QML.
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

    /**
     * @brief Returns the GemData object describing the gem.
     * @detail This method was not intended to be public, but public access is needed it for rendering.
     * @return
     */
    const GemData &data() const;

    /**
     * @brief Constructs normal matrix for gem in order to transform it from objectspace into worldsapce.
     * @return
     */
    const QMatrix4x4 &model() const;

    const QVector3D &position() const;
    virtual void setPosition(const QVector3D &position);

    /**
     * @brief Radius of boundingsphere. This value is influenced by scale and the geometry of gem.
     * @return
     */
    qreal radius() const;

    /**
     * @brief Rotation around own center.
     * @return
     */
    const QQuaternion &rotation() const;
    /**
     * @brief Sets the rotation of gem around own center.
     * @param rotation Value the roation will be set to.
     * @seealso setRotationFromEuler()
     */
    virtual void setRotation(const QQuaternion &rotation);
    /**
     * @brief Rotates the gem around the center of the gem
     * @param quaternion Specifies how the gem should be rotated.
     */
    void rotate(const QQuaternion &quaternion);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    /**
     * @brief Returns the type of gem, in order to differentiate between types even if you have only AbstractGems
     * @return
     */
    GemType type() const;

    /**
     * @brief Calculates distance to collision with gem's boundingsphere.
     * @detail The boundingsphere is specified by gems themself and cannot be influenced from outside. Because the collision point is only
     * calculated with the bondingsphere computation is pretty fast.
     * @param ray The ray which might collide with gem's boundingsphere
     * @param collisionPoint Optional parameter. If provided the collision point
     * of ray with boundingsphere is written into. If no collision occurs the maximum
     * float value is written into all components.
     * @return The factor you need to apply ray.direction() to ray.startPosition().
     * If no collision occurs maximum float value is returned. Do not compare this value with return values of intersectedBy()
     * @seealso intersectedBy()
     */
    float boundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    /**
     * @brief Calcualtes the distance to collision of ray with gem.
     * @detail This method calculates the real collision point. Therefore, many computations are done especially for complex gems.
     * @param ray The ray that might collide with gem.
     * @param collisionPoint Optional parameter. If a collision occurs the collision point will be written into this else all components contain maximum float values.
     * @return The factor you need to apply ray.normalizedDirection() to ray.startDirection().
     * If no collision occurs this value will be highest possible float. Do not compare this value with return value of boundingsphereIntersectedBy()
     * @seealso boundingSphereIntersectedBy()
     */
    virtual float intersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    /**
     * @brief Calculates all new rays, that will be created by a collision with that gem. Also affect gem attributes.
     * @param ray Ray that might collide.
     * @param scene The scene the ray and gem are in. This is needed in order to calculate new rays appropriately.
     * @return A List of light rays that should be added to scene. If no collision occurs, the list is empty. Ownership of rays contained in list is transferred to caller.
     */
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
     * @brief Finds face of gem intersected by given ray. Ownership of returned face is transferred to caller.
     * @param ray Ray that might intersect gem
     * @param intersectedFace A pointer to intersected face is written into. Because this triangle is in worldspace and for performance reasons the ownership of face is transferred to caller. If ray does not intersect nullptr is written.
     * @param collisionPoint Optional parameter. If the given pointer is not nullptr the collisionpoint is written into.
     * @return Returns distance to collisionpoint. If no collission occured the value is maximum of float.
     */
    float faceIntersectedBy(const LightRay &ray, Triangle *&intersectedFace, QVector3D *collisionPoint = nullptr);

    /**
     * @brief Calculates triangle in world coordinates for given triangle. Therefore, position, rotatition and scale of gem are used.
     * @param triangle Objectspace triangle whose corresponding worldspace triangle should be calculated.
     * @return Returns the Triangle in world coordinates.
     */
    Triangle inWorldCoordinates(const Triangle &triangle);

protected:
    GemData *m_data;
    qreal m_radius;
};

#endif // GEOMETRY_H
