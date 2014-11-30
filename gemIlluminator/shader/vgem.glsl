#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 modelViewProjection;
attribute highp vec3 a_vertex;
attribute highp vec3 a_color;
attribute highp vec3 a_normal;

varying vec3 v_color;

const vec3 lightPos = vec3(0.5, 0.8, 1.0);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main()
{
    gl_Position = modelViewProjection * vec4(a_vertex, 1.0);

    vec4 vertPos4 = modelViewProjection * vec4(a_vertex, 1.0);
    vec4 normal4 = modelViewProjection * vec4(a_normal, 1.0);
    vec3 normal = vec3(normal4);
    vec3 vertPos = vec3(vertPos4) / vertPos4.w;
    vec3 lightDir = normalize(lightPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-vertPos);

    float lambertian = max(dot(lightDir, normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
        float specAngle = max(dot(reflectDir, viewDir), 0.0);
        specular = pow(specAngle, 4.0);
    }

    v_color = lambertian * a_color + specular * specColor;
}
