#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp vec3 a_vertex;
attribute highp vec3 a_normal;
attribute highp float a_index;

uniform vec4 u_xyzs[128];
uniform vec4 u_rotation[128];
uniform vec4 u_rgba[128];

uniform float u_maxNumberOfGems;
uniform float u_sceneExtent;
uniform sampler2D u_data;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 modelViewProjection;
uniform mat4 viewProjection;
uniform vec3 eye;
uniform vec3 color;

varying vec3 v_color;
varying vec3 v_lightIntensity;
varying vec3 v_eyeVector;
varying vec3 v_normal;

const bool drawSingleGem = false;
const bool drawPackedGemsWithUiforms = false;
const bool drawTextureOptimization = true;
const vec4 lightPosition = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 kD = vec3(0.5, 0.5, 0.5);
const vec3 lD = vec3(0.8, 0.8, 0.8);

void singleGem()
{
    v_normal = normalize(normalMatrix * a_normal);
    vec4 eyeCoords = model * vec4(a_vertex, 1.0);
    v_eyeVector = eyeCoords.xyz - eye;

    vec3 s = normalize(vec3(lightPosition - eyeCoords));
    v_lightIntensity = lD * kD * max(dot(s, v_normal), 0.0);

    // TODO: At some point we'll add projection again
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);

    v_color = color;
}

vec3 rotateVector( vec4 quat, vec3 vec )
{
    return vec + 2.0 * cross(quat.xyz, cross(quat.xyz, vec) + quat.w * vec);
}

void packedGemsWithUniforms()
{
    int index = int(a_index);
    vec3 scaledCoord = a_vertex * u_xyzs[index].w;
    vec3 rotatedCoord = rotateVector(u_rotation[index], scaledCoord);
    vec3 worldCoord = rotatedCoord + u_xyzs[index].xyz;

    v_normal = normalize(rotateVector(u_rotation[index], a_normal));
    v_eyeVector = worldCoord - eye;

    vec3 s = normalize(lightPosition.xyz - worldCoord);
    v_lightIntensity = lD * kD * max(dot(s, v_normal), 0.0);

    v_color = u_rgba[index].xyz;
    gl_Position = viewProjection * vec4(worldCoord, 1.0);
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
    vec4 xyzsHigh = texture2D(u_data, vec2(0.125, index));
    vec4 xyzsLow = texture2D(u_data, vec2(0.375, index));
    vec4 xyzs = decode(xyzsHigh, xyzsLow, -u_sceneExtent, u_sceneExtent);
    vec4 rotation = texture2D(u_data, vec2(0.625, index));
    rotation = rotation * 2.0 - 1.0;
    vec4 rgb_ = texture2D(u_data, vec2(0.875, index));

    vec3 scaledCoord = a_vertex * xyzs.w;
    vec3 rotatedCoord = rotateVector(rotation, scaledCoord);
    vec3 worldCoord = rotatedCoord + xyzs.xyz;

    v_normal = normalize(rotateVector(rotation, a_normal));
    v_eyeVector = worldCoord - eye;

    vec3 s = normalize(lightPosition.xyz - worldCoord);
    v_lightIntensity = lD * kD * max(dot(s, v_normal), 0.0);

    v_color = rgb_.xyz;
    gl_Position = viewProjection * vec4(worldCoord, 1.0);
}

void main()
{
    if (drawSingleGem) {
        singleGem();
    } else if (drawPackedGemsWithUiforms) {
        packedGemsWithUniforms();
    } else if (drawTextureOptimization) {
        drawOptimizedWithTexture();
    }
}
