#ifndef SHADERPROGRAMS
#define SHADERPROGRAMS

inline uint qHash(ShaderPrograms key, uint seed)
{
    return qHash(static_cast<uint>(key), seed);
}

/**
 * @brief The ShaderPrograms enum
 * @detail Manage ShaderPrograms with the help of this enum class. If it is neccessary to pass several shader programgs at once,
 * these enums can be used for an explicit association.
 */
enum class ShaderPrograms {
    GemProgram,
    LighRayProgram,
    EnvMapProgram,
    SceneProgram,
    GaussHorizontalProgram,
    GaussVerticalProgram
};

#endif // SHADERPROGRAMS

