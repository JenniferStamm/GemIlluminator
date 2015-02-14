#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D u_sceneTexture;

varying vec2 v_uv;

void main()
{
     gl_FragColor = texture2D(u_sceneTexture, v_uv);
     //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
