#ifndef LIGHTRAYRENDERER_H
#define LIGHTRAYRENDERER_H

#include <QObject>

class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template<typename T> class QSet;
template<typename T> class QVector;

class LightRay;
class LightRayData;

/**
 * @brief The LightRayRenderer packs LightRays and paint them
 */
class LightRayRenderer : public QObject
{
    Q_OBJECT
public:
    explicit LightRayRenderer(QObject *parent = 0);
    virtual ~LightRayRenderer();

    /**
     * @brief Adds a new LightRay that will be rendered.
     * @detail Dynamic LightRays are added without checking wheter they exist or not.
     * Static LightRays will be only added if the LightRay is not allready drawn by LightRayRenderer
     * @param lightRay The LightRay that is added
     */
    void addLightRay(const LightRay &lightRay);
    /**
     * @brief Paints all LightRays
     * @param gl QOPenGLFunctions which are used for gl-calls
     * @param viewProjection The viewprojection matrix for drawing the rays.
     * @param shaderProgram The shader program that will be used to draw rays.
     */
    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);
    /**
     * @brief Removes all dynamic rays. It is suggested to do so every frame.
     */
    void resetDynamicRays();

protected:
    /**
     * @brief Calculates vertex data out of start and end position of a ray.
     * @param rayData The ray that should be drawn and requires vertex data
     * @param vertices A vector the vertex data will be appended
     * @param indices A vector the index data will be appended
     */
    void calculateVertexDataFor(const LightRayData & rayData, QVector<float> &vertices, QVector<unsigned int> & indices);
    void updateDynamicVBO();
    void updateStaticVBO();
    void updateRayVBO(QOpenGLBuffer *&vertexBuffer, QOpenGLBuffer *&indexBuffer, const QSet<LightRayData> &data);

protected:
    QOpenGLBuffer *m_dynamicIndexBuffer;
    QSet<LightRayData> *m_dynamicRays;
    QOpenGLBuffer *m_dynamicVertexBuffer;
    bool m_isStaticVBOUpdateRequired;
    QOpenGLBuffer *m_staticIndexBuffer;
    QSet<LightRayData> *m_staticRays;
    QOpenGLBuffer *m_staticVertexBuffer;
};

#endif // LIGHTRAYRENDERER_H
