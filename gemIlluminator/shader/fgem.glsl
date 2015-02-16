#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform samplerCube envmap;

varying vec3 v_color;
varying vec3 v_eyeVector;
varying vec3 v_normal;

void main()
{
    vec3 reflectVector = reflect(v_eyeVector, v_normal);
    vec4 reflectColor = textureCube(envmap, reflectVector);

    vec3 refractVector = refract(v_eyeVector, v_normal, 1.4);
    vec4 refractColor = textureCube(envmap, refractVector);

    vec3 fresnel = mix(reflectColor.xyz, refractColor.xyz, clamp(dot(v_normal, v_eyeVector), 0.0, 1.0));

    gl_FragColor = vec4(mix(fresnel, v_color, 0.2), 1.0);
}
