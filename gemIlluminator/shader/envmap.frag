#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform samplerCube cubemap;

// attribute vec4 fragColor;

varying vec3 v_eye;

void main()
{
	vec3 eye = normalize(v_eye);
        gl_FragColor = textureCube(cubemap, eye);
}
