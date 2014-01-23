#version 330

layout(location = 0) in  vec3  in_Position;
layout(location = 1) in  vec3  in_Normal;
layout(location = 2) in  vec2  in_texCoord;

uniform mat4 camTrans;

out VertexData {
    vec2 texCoord;
} VertexOut;

void main(void)
{
	VertexOut.texCoord = in_texCoord;

    gl_Position = camTrans * vec4(in_Position,1.0f);
}
