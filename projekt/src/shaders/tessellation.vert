#version 430 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

uniform mat4 mvp;
uniform vec3 camPos;

out VertexData {
	vec3 world_pos_tcs;
} VertexOut;

void main(void)
{
	//vec4 pos = mvp * vec4(in_Position, 1.0f); // Unecessary because need to transform all new verts in TES anyway.
	//VertexOut.world_pos_tcs = in_Position;

	//gl_Position = vec4(in_Position, 1.0f);// pos;
	//gl_Position = pos;
	gl_Position = vec4(in_Position, 1.0f);
}
