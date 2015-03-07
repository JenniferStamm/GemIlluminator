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
varying vec3 v_eyeVector;
varying vec3 v_normal;
varying vec3 v_vertex;

const vec4 lightDirection = vec4(4.0, 8.0, 4.0, 1.0);
const vec3 specularColor = vec3(1.0, 1.0, 1.0);
const float brightness = 0.8;
const float shininess = 16.0;
//const float refractEnvMapRatio = 0.0;
const float dispersionStrength = 1.0;

vec3 transmissionTerm(vec3 eyeVector)
{
    vec3 refractVector1 = refract(eyeVector, v_normal, 0.9);
    vec3 refractVector2 = refract(eyeVector, v_normal, 0.7);

    vec3 randomVector = normalize(vec3(mod(v_index * 289.0, 1021.0)));
    refractVector2 = reflect(refractVector2, randomVector);

    vec3 refractColor = textureCube(refractionMap, refractVector1).xyz;
    refractColor += textureCube(refractionMap, refractVector2.yxz).xyz;

    refractColor = pow(refractColor, vec3(4.0));

    return refractColor * brightness;
}

vec3 reflectionTerm(vec3 eyeVector)
{
    vec3 r_face = reflect(eyeVector, v_normal);
    //vec3 r_curved = reflect(eyeVector, v_averagedNormal);

    float RdotL = clamp(dot(r_face, normalize(lightDirection.xyz)), 0.0, 1.0);
    vec3 specular = pow(RdotL, shininess) * specularColor;

    float fresnel = pow(1.0 - clamp(dot(v_normal, eyeVector), 0.0, 1.0), 2.0);

    vec3 environmentColor = textureCube(envmap, r_face).xyz; // could use r_curved
    vec3 rainbowColor = textureCube(rainbowMap, r_face).xyz; // could use r_curved

    rainbowColor = mix(vec3(1.0), rainbowColor, dispersionStrength);
    environmentColor = mix(environmentColor, rainbowColor, vec3(0.1)) * fresnel;

    return clamp(specular, 0.0, 1.0) + environmentColor;
}

void main()
{
    vec3 eyeVector = normalize(v_eyeVector);
    vec3 color = mix(reflectionTerm(eyeVector), transmissionTerm(eyeVector), vec3(0.1));
    gl_FragColor = vec4(mix(color, v_color, 0.5), 1.0);
    //gl_FragColor = vec4(textureCube(rainbowMap, eyeVector).xyz, 1.0);
}
