#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp vec3 a_vertex;
attribute highp vec3 a_color;
attribute highp vec3 a_normal;

uniform mat4 model;
uniform mat4 modelIT;
uniform mat4 modelViewProjection;

varying vec3 v_color;
varying float v_specularIntensity;
varying float v_diffuseIntensity;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);

void main()
{
    // TODO: At some point we'll add projection again
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
    v_color = a_color;

    vec4 vertexPositionCamera = model * vec4(a_vertex, 1.0);
    vec3 normalCamera = vec3(normalize(modelIT * vec4(a_vertex, 1.0)));
    vec4 lightPositionCamera = model * lightDirection;

    vec3 lightVertex = normalize(vec3(lightPositionCamera - vertexPositionCamera));
    vec3 lightReflection = normalize(reflect(lightVertex, normalCamera));

    // diffuse light
    v_diffuseIntensity = max(dot(lightVertex, normalCamera), 0.0);

    // specular light
    v_specularIntensity = max(dot(lightReflection, normalize(vec3(vertexPositionCamera))), 0.0);
    v_specularIntensity = pow(v_specularIntensity, 6.0);
}
