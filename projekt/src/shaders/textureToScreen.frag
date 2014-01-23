#version 330

in VertexData {
    vec2 texCoord;
} FragIn;

uniform sampler2D texUnit;

out vec4 out_Color;


void main(void)
{
	//vec4 zval = texture(texUnit, FragIn.texCoord);
	//out_Color = vec4(zval.r, zval.g, zval.b, 1.f);
	out_Color = vec4(1.f);
}