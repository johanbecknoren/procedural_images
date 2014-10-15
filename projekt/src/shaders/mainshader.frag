#version 330

in VertexData {
	vec3 pos;
	vec3 normal;
    vec2 texCoord;
} FragIn;

out vec4 out_Color;

const vec3 lightDir = normalize(vec3(1.f, 1.f,0.f));

void main(void)
{
	out_Color = vec4(1,0,0,1)*dot(lightDir, normalize(FragIn.normal));
	//out_Color = vec4(normalize((FragIn.normal)), 1.0f);
	//out_Color = vec4(FragIn.normal, 1.0f);
	//out_Color = vec4(1.0f);
}