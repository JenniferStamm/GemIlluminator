#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform samplerCube envMap;
uniform samplerCube gemStructureMap;
uniform samplerCube rainbowMap;

varying vec3 v_color;
varying float v_index;
varying vec3 v_eyeVector;
varying vec3 v_normal;
varying vec3 v_vertex;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const float coordinateObfuscator = 0.8;
const float shininess = 16.0;
const float smoothnessFactor = 0.5;

vec3 envMapCoordinates(vec3 eyeVector)
{
    vec3 refractVector = refract(eyeVector, v_normal, 0.9);

    vec3 randomVector = normalize(vec3(mod(v_index * 289.0, 1021.0)));
    refractVector = reflect(refractVector, randomVector);

    vec3 envMapCoordinates = textureCube(gemStructureMap, v_vertex).xyz * coordinateObfuscator;
    envMapCoordinates += textureCube(rainbowMap, refractVector).xyz;

    return envMapCoordinates;
}

vec3 calculateEnvironmentColor(vec3 eyeVector)
{
    vec3 r_face = reflect(eyeVector, v_normal);

    float RdotL = clamp(dot(r_face, normalize(lightDirection.xyz)), 0.0, 1.0);
    vec3 specular = pow(RdotL, shininess) * specularColor;

    float fresnel = pow(1.0 - clamp(dot(v_normal, eyeVector), 0.0, 1.0), 2.0);

    vec3 envMapAccess = mix(envMapCoordinates(eyeVector), r_face, vec3(smoothnessFactor));
    vec3 environmentColor = textureCube(envMap, envMapAccess).xyz;
    vec3 rainbowColor = textureCube(rainbowMap, r_face).xyz;

    environmentColor = mix(environmentColor, rainbowColor, vec3(0.1)) * fresnel;

    return clamp(specular, 0.0, 1.0) + environmentColor;
}

void main()
{
    vec3 eyeVector = normalize(v_eyeVector);
    vec3 color = mix(calculateEnvironmentColor(eyeVector), textureCube(gemStructureMap, v_vertex).xyz, vec3(0.1));
    gl_FragColor = vec4(mix(color, v_color, 0.3), 1.0);
}
