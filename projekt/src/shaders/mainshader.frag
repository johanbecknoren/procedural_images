#version 330

in VertexData {
    vec2 texCoord;
} FragIn;

out vec4 out_Color;


void main(void)
{
	out_Color = vec4(0.8f);
}