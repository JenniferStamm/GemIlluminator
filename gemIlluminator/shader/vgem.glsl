#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp vec3 a_vertex;
attribute highp vec3 a_normal;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjection;
uniform vec3 color;

varying vec3 v_color;
varying vec3 v_lightIntensity;

const vec4 lightPosition = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 kD = vec3(0.5, 0.5, 0.5);
const vec3 lD = vec3(0.8, 0.8, 0.8);

void main()
{
    vec3 tnorm = normalize(mat3(normalMatrix) * a_normal);
    vec4 eyeCoords = model * vec4(a_vertex, 1.0);

    vec3 s = normalize(vec3(lightPosition - eyeCoords));
    v_lightIntensity = lD * kD * max(dot(s, tnorm), 0.0);

    // TODO: At some point we'll add projection again
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
    v_color = color;
    v_color = tnorm * 0.5 + 0.5;
}
