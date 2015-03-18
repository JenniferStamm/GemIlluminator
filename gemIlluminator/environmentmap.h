#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Camera;
class CubeMap;
class ScreenAlignedQuad;

/**
 * @brief The EnvironmentMap is a CubeMap based rendering technique for showing some scene enviroment.
 */
class EnvironmentMap : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new EnvironmentMap. The specified cube map textures are loaded immediately.
     * @param gl The QOpenGLFunctions which are used for every gl-call
     * @param cubeMapPrefix The name of cubemap that should be shown.
     * @param parent QObject-parent
     */
    EnvironmentMap(QOpenGLFunctions &gl, QString cubeMapPrefix, QObject *parent = 0);
    ~EnvironmentMap();

    /**
     * @brief Paints environmentmap into the scene using a ScreenAlignedQuad.
     * @param camera The camera which is used to render the scene
     */
    void paint(const Camera &camera);

    /**
     * @brief Replace current CubeMap with new one specified by name.
     * @param newCubeMapPrefix The name of the CubeMap
     * @see CubeMap::update()
     */
    void update(QString newCubeMapPrefix);

    /**
     * @brief OpenGL texture that is used for drawing.
     * @return
     */
    uint cubeMapTexture();

protected:
    void initialize();
    void initializeShaderProgram();

protected:
    CubeMap *m_cubeMap;
    bool m_initialized;
    QOpenGLFunctions &m_gl;
    ScreenAlignedQuad *m_quad;
    QOpenGLShaderProgram *m_shaderProgram;
};

#endif // ENVIRONMENTMAP_H
