uniform mat4 modelViewProjection;
attribute highp vec3 a_vertex;
void main()
{
	gl_Position = modelViewProjection * vec4(a_vertex, 1.0);
}