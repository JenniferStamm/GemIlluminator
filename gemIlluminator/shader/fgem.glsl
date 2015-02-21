#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

//attribute highp float a_index;

uniform samplerCube envmap;
uniform samplerCube refractionMap;
uniform samplerCube rainbowMap;

varying vec3 v_color;
varying float v_index;
varying vec3 v_lightIntensity;
varying vec3 v_eyeVector;
varying vec3 v_normal;
varying vec3 v_vertex;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
//const float refractEnvMapRatio = 0.0;
const float dispersionStrength = 1.0;

vec3 transmissionTerm()
{
    vec3 refractVector1 = refract(v_eyeVector, v_normal, 2.4);
    vec3 refractVector2 = refract(v_eyeVector, v_normal, 1.8);

    vec3 randomVector = normalize(vec3(mod(v_index * 289.0, 1021.0)));
    refractVector2 = reflect(refractVector2, randomVector);

    vec3 refractColor = textureCube(refractionMap, refractVector1).xyz * 2; // Added * 2
    refractColor += textureCube(refractionMap, refractVector2.yxz).xyz * 2;

    refractColor = pow(refractColor, 4.0);

    return refractColor;
}

vec3 reflectionTerm()
{
    vec3 r_face = reflect(v_eyeVector, v_normal);
    //vec3 r_curved = reflect(v_eyeVector, v_averagedNormal);

    float RdotL = clamp(dot(r_face, normalize(lightDirection.xyz)), 0.0, 1.0);
    vec3 specular = pow(RdotL, shininess) * specularColor;

    float fresnel = pow(1.0 - clamp(dot(v_normal, v_eyeVector), 0.0, 1.0), 2.0);

    vec3 environmentColor = textureCube(envmap, r_face); // could use r_curved
    vec3 rainbowColor = textureCube(rainbowMap, r_face); // could use r_curved

    rainbowColor = mix(1.0, rainbowColor, dispersionStrength);
    environmentColor = mix(environmentColor, rainbowColor, 0.1) * fresnel;

    return clamp(specular, 0.0, 1.0) + environmentColor;
}

void main()
{
    //vec4 refractColor = mix(textureCube(refractionMap, refractVector), textureCube(envmap, refractVector), refractEnvMapRatio);

    //vec3 fresnel = mix(reflectColor.xyz, refractColor.xyz, clamp(dot(normalize(v_normal), normalize(v_eyeVector)), 0.0, 1.0));

    //gl_FragColor = vec4(mix(fresnel, v_color, 0.2), 1.0);

    vec3 color = reflectionTerm();
    gl_FragColor = vec4(color, 1.0);
}
