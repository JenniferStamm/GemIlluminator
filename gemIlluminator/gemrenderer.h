#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

template<typename Key, typename T> class QHash;
template<typename T> class QList;
class QMatrix4x4;
class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QOpenGLTexture;
template<typename T> class QVector;

class AbstractGem;
class GemData;
enum class GemType;

/**
 * @brief The GemRenderer class renders all of our gems.
 * @detail For performance reasons the GemRenderer packs all gems and fakes instanced drawing.
 */
class GemRenderer
{
    /**
     * @brief The GemDataInfo class stores the GemData of a Gem. In addition some further values are stored.
     * @detail Because a index is stored, we do not want this accessiable from outside of GemRenderer and GemRenderData is a private nested class of GemRenderer.
     */
    class GemDataInfo
    {
    public:
        GemDataInfo();
        ~GemDataInfo();

        /**
         * @brief Data of represented gem
         * @return
         */
        const GemData &data() const;
        /**
         * @brief Set data() to value.
         * @param data
         */
        void setData(const GemData &data);

        /**
         * @brief The index is used to identify the position of gem within texture. Also this value is written as additional attribute to attribute data.
         * @return
         */
        int index() const;
        /**
         * @brief Sets value of index()
         * @param index
         */
        void setIndex(int index);

        /**
         * @brief The number of vertices the gem contains
         * @return
         */
        int numberOfVertices();

        /**
         * @brief Writes all attribute data (vertices and normal) of gem in a vector with the index following each vertex
         * @param vector
         */
        void appendVerticesWithIndexTo(QVector<float> &vector) const;

    protected:
        GemData *m_data;
        int m_index;
    };

    /**
     * @brief The GemRenderData class renders one type of gems.
     * @detail Originally the idea was to bundle required buffers, textures and some additional information (e.g. how many gems are stored).
     * This is also a private nested class, because we need access to GemDataInfo
     */
    class GemRenderData
    {
    public:
        GemRenderData();
        ~GemRenderData();

        /**
         * @brief Removes all gpu ressources.
         * @param gl QOpenGLFunctions used to do gl-calls
         */
        void cleanup(QOpenGLFunctions &gl);
        /**
         * @brief Initialize all gpu ressources.
         * @param gl QOpenGLFunctions used to do gl-calls
         */
        void initialize(QOpenGLFunctions &gl);

        /**
         * @brief Renders all gems of a kind with one rendering call.
         * @param gl QOpenGLFunctions used to do gl-calls
         * @param program The program that is used to render the gems.
         */
        void paint(QOpenGLFunctions &gl, QOpenGLShaderProgram &program);

        /**
         * @brief Adds a gem to GemRenderData or updates the gem if allredy contained by GemRenderData.
         * @detail The decision if a gem is allready contained is done by pointer value of gem
         * @param gem The gem that should be added or updated
         * @param gl QOpenGLFunctions used to do gl-calls
         */
        void addOrUpdateGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        /**
         * @brief Sets the number of vertices per gem.
         * @param numberOfVertices
         */
        void setVerticesPerGem(int numberOfVertices);
        /**
         * @brief Sets the scene extent of scene. This value is required to encode the position to byte-textures.
         * @param extent
         */
        void setSceneExtent(float extent);
        /**
         * @brief Sets if float textures can be used or not.
         * @param enable
         */
        void setFloatTexturesEnabled(bool enable);
        /**
         * @brief Append per gem attributes to vector.
         * @param gem The gem, which attributes should be appended.
         * @param vector The vector the data is appended to.
         */
        void appendAttributesToVector(GemDataInfo *gem, QVector<float> &vector);
        /**
         * @brief Append per gem attributes to vector.
         * @detail This overrided version encodes the data into unsigned chars in order to support byte textures.
         * @param gem The gem, which attributes should be appended.
         * @param vector The vector the data is appended to.
         */
        void appendAttributesToVector(GemDataInfo *gem, QVector<unsigned char> &vector);

    protected:
        /**
         * @brief Add a new gem.
         * @detail In order to save allocation time we allocate more memory then needed and reorganize our data if we needs even more.
         * @param gem The gem that will be added
         * @param gl QOpenGLFunctions used to do gl-calls
         */
        void addGem(GemDataInfo *gem, QOpenGLFunctions &gl);
        /**
         * @brief Updates gem.
         * @param gem The gem that will be updated.
         * @param gl QOpenGLFunctions used to do gl-calls
         */
        void updateGem(GemDataInfo *gem, QOpenGLFunctions &gl);

    protected:
        int m_allocatedGems;
        int m_allocatedAndUsedGems;
        bool m_areFloatTexturesAvailable;
        unsigned int m_dataBuffer;
        QList<GemDataInfo *> *m_gems;
        bool m_hasErrorOccured;
        bool m_isInitialized;
        unsigned int m_lowestUnusedIndex;
        int m_maxTextureSize;
        float m_sceneExtent;
        int m_texelPerGem;
        QOpenGLBuffer *m_vertexBuffer;
        int m_verticesPerGem;
    };


public:
    GemRenderer();
    ~GemRenderer();

    /**
     * @brief Cleans up all gpu ressources.
     * @param gl QOpenGLFunctions used to do gl-calls
     */
    void cleanup(QOpenGLFunctions &gl);
    /**
     * @brief Initializes all gpu ressources
     * @param gl QOpenGLFunctions used to do gl-calls
     */
    void initialize(QOpenGLFunctions &gl);
    /**
     * @brief paint Paints all gems. Using faked instanced drawing.
     * @param gl QOpenGLFunctions used to do gl-calls
     * @param program The program that is used for drawing gems.
     */
    void paint(QOpenGLFunctions &gl, QOpenGLShaderProgram &program);
    /**
     * @brief Sets the scene extent, which is required to encode gem positions into byte textures
     * @param extent
     */
    void setSceneExtent(float extent);
    /**
     * @brief Save given gem in order to update it later.
     * @param gem The gem that should be updated (or added).
     * @see updateData()
     */
    void updateGem(AbstractGem *gem);

protected:
    /**
     * @brief Updates previous added gem using updateGem()
     * @param gl QOpenGLFunctions used to do gl-calls
     */
    void updateData(QOpenGLFunctions &gl);

protected:
    bool m_areFloatTexturesAvailable;
    QHash<GemType, GemRenderData *> *m_gemBuffersTex;
    QHash<AbstractGem *, GemDataInfo *> *m_gemMap;
    bool m_isGemBufferUpdateRequired;
    bool m_isGemDataBufferInvalid;
    bool m_isInitialized;
    QList<GemDataInfo *> *m_newGems;
    float m_sceneExtent;
};

#endif // GEMRENDERERNEW_H
