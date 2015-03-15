#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;
class QTime;

class Camera;
class BlurEffect;
class CubeMap;
class EnvironmentMap;
class PainterQML;
class ScreenAlignedQuad;
class Scene;
class SceneRenderer;
enum class ShaderPrograms;

/**
 * @brief The Painter class
 * @detail Includes the rendering process, thus creates the whole picture. The Painter will be used by QML within rendering thread
 *
 */
class Painter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor of Painter
     * @param painter Corresponding QML-Painter. Will be informed about finished rendering.
     * @param parent QObject-parent
     */
    explicit Painter(PainterQML *painter, QObject *parent = 0);

    /**
     * @brief Destrutor. Will delete all rendering related classes and ressources.
     */
    virtual ~Painter();

    /**
     * @brief Updates enviorment map using config file
     */
    void updateEnvMap();

    /**
     * @brief Check if Painter is active. Active means the painter is rendering.
     * @return Active state of painter.
     */
    bool isActive() const;
    /**
     * @brief Set Active state. If active is true the painter renders the picture.
     * @param active
     */
    void setActive(bool active);

    /**
     * @brief Clears the scene and removes all not anymore required ressources.
     */
    void clearScene();

    /**
     * @brief Painter copies all needed information of scene into own thread.
     * @param scene The scene that should be synchronized. If nullptr is passed it is like clearScene()
     */
    void synchronizeScene(Scene *scene);

public slots:
    /**
     * @brief Starts rendering and paints the whole picture.
     */
    void paint();

signals:
    /**
     * @brief This signal is emitted if initialization of all required ressources for current scene are initalized.
     * @detail The signal is everytime emitted if previous scene was deleted and a new scene was synchronized.
     */
    void initializeDone();

protected:
    /**
     * @brief Initializes all required ressources,
     * that are direct members of painter (scene independant ressources).
     * This method is executed within first paint.
     */
    void initialize();
    void initializeFBOs();
    void initializeShaderPrograms();
    void renderLightRays(const Camera &camera);
    void renderScene(const Camera &camera);

protected slots:
    void handleInitializeDone();

protected:
    bool m_active;
    EnvironmentMap *m_envMap;
    QOpenGLFunctions *m_gl;
    bool m_initialized;

    BlurEffect *m_blurEffectScene;
    int m_blurViewportRatioScene;
    uint m_glowSceneFBO;
    uint m_glowSceneDepthRB;
    uint m_glowSceneTexture;

    BlurEffect *m_blurEffectPreviewScene;
    int m_blurViewportRatioPreviewScene;
    uint m_glowPreviewSceneFBO;
    uint m_glowPreviewSceneDepthRB;
    uint m_glowPreviewSceneTexture;

    uint m_previewSceneFBO;
    uint m_previewSceneDepthRB;
    uint m_previewSceneTexture;

    PainterQML *m_painterQML;
    ScreenAlignedQuad *m_quad;

    CubeMap *m_rainbowMap;
    CubeMap *m_gemStructureMap;

    uint m_sceneFBO;
    uint m_sceneDepthRB;
    uint m_sceneTexture;

    QHash<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    QSize *m_usedViewport;

    int m_counter;
    int m_oldElapsed;
    QTime *m_time;

    SceneRenderer *m_sceneRenderer;
    Camera *m_camera;
    Camera *m_previewCamera;
};

#endif // PAINTER_H
