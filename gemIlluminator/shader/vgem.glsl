#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp vec3 a_vertex;
attribute highp vec3 a_normal;
attribute highp float a_index;

uniform float u_texWidth;
uniform float u_texHeight;
uniform float u_sceneExtent;
uniform sampler2D u_data;
uniform bool u_isFloatTextureAvailable;
uniform float u_maxGemSize;
uniform float u_minGemSize;

uniform mat4 viewProjection;
uniform vec3 eye;

varying vec3 v_color;
varying vec3 v_eyeVector;
varying float v_index;
varying vec3 v_normal;
varying vec3 v_vertex;

vec2 getUVOfGemAttribute(float gemIndex, float attributeIndex)
{
    float texelPerGem;
    if (u_isFloatTextureAvailable) {
        texelPerGem = 3.0;
    } else {
        texelPerGem = 4.0;
    }
    float bufferPosX = mod(gemIndex * texelPerGem, u_texWidth) + attributeIndex;
    float bufferPosY = floor((gemIndex * texelPerGem) / u_texWidth);
    vec2 result = vec2(bufferPosX, bufferPosY);
    result += vec2(0.5);
    result /= vec2(u_texWidth, u_texHeight);
    return result;
}

vec3 rotateVector( vec4 quat, vec3 vec )
{
    return vec + 2.0 * cross(quat.xyz, cross(quat.xyz, vec) + quat.w * vec);
}

vec4 decode(vec4 high, vec4 low, float minBorder, float maxBorder)
{
    float scaleHigh = maxBorder - minBorder;
    float scaleLow = (maxBorder - minBorder) / 256.0;

    vec4 scaled = high * scaleHigh + low * scaleLow;
    return scaled + minBorder;
}

void drawOptimizedWithTexture()
{
    //texture coordinates have to point to center of texel not border (u-coordinates: precalculated for width)
    vec4 xyzs;
    vec4 rotation;
    vec4 rgb_;
    if (u_isFloatTextureAvailable) {
        xyzs = texture2D(u_data, getUVOfGemAttribute(a_index, 0.0));
        rotation = texture2D(u_data, getUVOfGemAttribute(a_index, 1.0));
        rgb_ = texture2D(u_data, getUVOfGemAttribute(a_index, 2.0));
    } else {
        vec4 xyzsHigh = texture2D(u_data, getUVOfGemAttribute(a_index, 0.0));
        vec4 xyzsLow = texture2D(u_data, getUVOfGemAttribute(a_index, 1.0));
        xyzs.xyz = decode(xyzsHigh, xyzsLow, -u_sceneExtent, u_sceneExtent).xyz;
        xyzs.w = decode(xyzsHigh, xyzsLow, u_minGemSize, u_maxGemSize).w;
        rotation = texture2D(u_data, getUVOfGemAttribute(a_index, 2.0));
        rotation = rotation * 2.0 - 1.0;
        rgb_ = texture2D(u_data, getUVOfGemAttribute(a_index, 3.0));
    }
    vec3 scaledCoord = a_vertex * xyzs.w;
    vec3 rotatedCoord = rotateVector(rotation, scaledCoord);
    vec3 worldCoord = rotatedCoord + xyzs.xyz;

    v_normal = normalize(rotateVector(rotation, a_normal));
    v_eyeVector = worldCoord - eye;

    v_color = rgb_.xyz;
    gl_Position = viewProjection * vec4(worldCoord, 1.0);
}

void main()
{
    v_index = a_index;
    v_vertex = a_vertex;
    drawOptimizedWithTexture();
}
