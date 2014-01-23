#version 330

in VertexData {
    vec2 texCoord;
} FragIn;

uniform sampler2D texUnit;

out vec4 out_Color;


void main(void)
{
	vec4 texval = texture(texUnit, FragIn.texCoord);
	out_Color = vec4(texval.rgb, 1.f);
	//out_Color = vec4(0.8f);
}