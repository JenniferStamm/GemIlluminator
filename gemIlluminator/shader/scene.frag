#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D sceneTexture;

varying vec2 v_uv;

void main()
{
        gl_FragColor = texture2D(sceneTexture, v_uv);
}
