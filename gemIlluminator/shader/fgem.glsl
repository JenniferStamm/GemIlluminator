#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute highp float a_index;

uniform samplerCube envmap;
uniform samplerCube refractionMap;

varying vec3 v_color;
varying vec3 v_lightIntensity;
varying vec3 v_eyeVector;
varying vec3 v_normal;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const float refractEnvMapRatio = 0.0;

void main()
{
    vec3 reflectVector = reflect(v_eyeVector, v_normal);
    vec4 reflectColor = textureCube(envmap, reflectVector);

    vec3 refractVector1 = refract(v_eyeVector, v_normal, 2.4);
    vec3 refractVector2 = refract(v_eyeVector, v_normal, 1.8);

    vec3 randomVector = normalize(vec3(a_index));
    refractVector2 = reflect(refractVector2, randomVector);

    vec3 refractColor = textureCube(refractionMap, refractVector1).xyz + textureCube(refractionMap, refractVector2).xyz;

    refractColor = pow(refractColor, 4.0);
    //vec4 refractColor = mix(textureCube(refractionMap, refractVector), textureCube(envmap, refractVector), refractEnvMapRatio);

    vec3 fresnel = mix(reflectColor.xyz, refractColor.xyz, clamp(dot(normalize(v_normal), normalize(v_eyeVector)), 0.0, 1.0));

    gl_FragColor = vec4(mix(fresnel, v_color, 0.2), 1.0);
}
