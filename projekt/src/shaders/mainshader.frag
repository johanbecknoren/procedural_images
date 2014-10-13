#version 330

in VertexData {
	vec3 pos;
	vec3 normal;
    vec2 texCoord;
} FragIn;

out vec4 out_Color;


void main(void)
{
	out_Color = vec4(FragIn.pos, 1.0f);
	//out_Color = vec4(FragIn.normal, 1.0f);
	//out_Color = vec4(1.0f);
}