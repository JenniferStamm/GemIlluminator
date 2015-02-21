#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp vec3 a_vertex;
attribute highp vec3 a_normal;
attribute highp float a_index;

uniform float u_maxNumberOfGems;
uniform float u_sceneExtent;
uniform sampler2D u_data;
uniform bool u_isFloatTextureAvailable;

uniform mat4 viewProjection;
uniform vec3 eye;

varying vec3 v_color;
varying vec3 v_eyeVector;
varying vec3 v_normal;

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
    float index = (a_index + 0.5) / (u_maxNumberOfGems);
    vec4 xyzs;
    vec4 rotation;
    vec4 rgb_;
    if (u_isFloatTextureAvailable) {
        xyzs = texture2D(u_data, vec2(0.167, index));
        rotation = texture2D(u_data, vec2(0.5, index));
        rgb_ = texture2D(u_data, vec2(0.833, index));
    } else {
        vec4 xyzsHigh = texture2D(u_data, vec2(0.125, index));
        vec4 xyzsLow = texture2D(u_data, vec2(0.375, index));
        xyzs = decode(xyzsHigh, xyzsLow, -u_sceneExtent, u_sceneExtent);
        rotation = texture2D(u_data, vec2(0.625, index));
        rotation = rotation * 2.0 - 1.0;
        rgb_ = texture2D(u_data, vec2(0.875, index));
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
    drawOptimizedWithTexture();
}
