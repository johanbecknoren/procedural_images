#version 330

layout(location = 0) in  vec3  in_Position;
layout(location = 1) in  vec3  in_Normal;
layout(location = 2) in  vec2  in_texCoord;

uniform mat4 camTrans;

out VertexData {
	vec3 pos;
	vec3 normal;
    vec2 texCoord;
} VertexOut;

void main(void)
{
	vec4 pos = camTrans * vec4(in_Position,1.0f);

	VertexOut.texCoord = in_texCoord;
	VertexOut.normal = in_Normal;
	//VertexOut.normal = camTrans * vec4(in_Normal,1.0f);
    VertexOut.pos = in_Position;

    gl_Position = pos;
}
