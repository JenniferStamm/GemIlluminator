#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QSize>
#include <QVector3D>

class QMatrix4x4;

/**
 * @brief The Camera class provides view and perspective projection matrices. Additional the viewport of camera is stored.
 * @detail The view of camera has to be specified by eye, center and up or by position, viewdirection and up.
 * It is allowed to mix both definitions, but it might lead to unexpected behaviour.
 * The perspective projection is specified by field of view, viewport, and near and far plane
 */
class Camera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D &eye READ eye WRITE setEye)
    Q_PROPERTY(const QVector3D &center READ center WRITE setCenter)
    Q_PROPERTY(const QVector3D &up READ up WRITE setUp)
    Q_PROPERTY(const QVector3D &position READ position WRITE setPosition)
    Q_PROPERTY(const QVector3D &viewDirection READ viewDirection WRITE setViewDirection)
    Q_PROPERTY(const QSize &viewport READ viewport WRITE setViewport)
    Q_PROPERTY(qreal zNear READ zNear WRITE setZNear)
    Q_PROPERTY(qreal zFar READ zFar WRITE setZFar)
    Q_PROPERTY(qreal fovy READ fovy WRITE setFovy)

public:
    explicit Camera(QObject *parent = 0);

    /** \brief Creates a new Camera with matrices copied.
     *  \param camera Specifies the camera the matrices are copied from.
     *  \param parent Specifies the parent
     */
    Camera(const Camera &camera, QObject *parent = 0);
    virtual ~Camera();

    const QMatrix4x4 &view() const;
    const QMatrix4x4 &viewInverted() const;
    const QMatrix4x4 &viewProjection() const;
    const QMatrix4x4 &viewProjectionInverted() const;
    const QMatrix4x4 &projection() const;
    const QMatrix4x4 &projectionInverted() const;

    const QVector3D &position() const;
    QVector3D viewDirection() const;
    const QVector3D &center() const;
    const QVector3D &eye() const;
    const QVector3D &up() const;

    const QSize &viewport() const;
    float fovy() const;
    float zNear() const;
    float zFar() const;

signals:
    void viewChanged();

public slots:
    /**
     * @brief Sets position of camera to given value. center() will be changed in order to keep viewDirection() the same.
     * @param position Camera will be set to this position.
     */
    void setPosition(const QVector3D &position);
    /**
     * @brief Sets viewDirection of camera to given vector. center() will be changed in order to
     * @param viewDirection Direction the camera should look.
     */
    void setViewDirection(const QVector3D &viewDirection);
    /**
     * @brief Sets eye (position) of camera to given value. center() will not be changed, so viewDirection() is set to center() - eye()
     * @param eye Camera will be set to this position.
     */
    void setEye(const QVector3D &eye);
    /**
     * @brief Sets center of camera to given value. viewDirection() will be set to new center() - eye().
     * @param center Position the camera is looking to.
     */
    void setCenter(const QVector3D &center);
    /**
     * @brief Sets up-vector of camera.
     * @param up New up vector.
     */
    void setUp(const QVector3D &up);
    /**
     * @brief Convenience method to specify view with one method call
     * @param eye See setEye()
     * @param center See setCenter()
     * @param up See setUp()
     */
    void setView(const QVector3D &eye, const QVector3D &center, const QVector3D &up);

    void setViewport(const QSize &viewport);
    void setViewport(int x, int y);
    void setFovy(float angle);
    void setZNear(float zNear);
    void setZFar(float zFar);

protected:
    void invalidateView() const;
    void invalidateProjection() const;
    void recalculateView() const;
    void recalculateProjection() const;
    void recalculateViewProjection() const;

protected:
    QVector3D *m_eye;
    QVector3D *m_center;
    QVector3D *m_up;
    QSize *m_viewport;
    float m_zNear;
    float m_zFar;
    float m_fovy;

    mutable QMatrix4x4 *m_view;
    mutable QMatrix4x4 *m_viewInverted;
    mutable QMatrix4x4 *m_projection;
    mutable QMatrix4x4 *m_projectionInverted;
    mutable QMatrix4x4 *m_viewProjection;
    mutable QMatrix4x4 *m_viewProjectionInverted;

    mutable bool m_isViewInvalid;
    mutable bool m_isProjectionInvalid;
    mutable bool m_isViewProjectionInvalid;
};

#endif // CAMERA_H
