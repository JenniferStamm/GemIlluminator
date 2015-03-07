#ifndef SHADERPROGRAMS
#define SHADERPROGRAMS

inline uint qHash(ShaderPrograms key, uint seed)
{
    return qHash(static_cast<uint>(key), seed);
}

enum class ShaderPrograms {
    GemProgram,
    LighRayProgram,
    EnvMapProgram,
    SceneProgram,
    GaussHorizontalProgram,
    GaussVerticalProgram
};

#endif // SHADERPROGRAMS

