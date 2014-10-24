#version 330

in VertexData {
	vec3 pos;
	vec3 normal;
    vec2 texCoord;
    vec3 viewDir;
    float height;
} FragIn;

out vec4 out_Color;

const vec3 lightDir = normalize(vec3(1.f, 0.5f,0.f));
const vec4 lightColor = vec4(0,1.0,0,1); // green
const vec4 ambientLightColor = vec4(0.6,0.6,1,1); // white

void main(void)
{
	//if(dot(FragIn.viewDir, FragIn.normal)>0)
	//	discard;
	float dotprod = (dot(lightDir, FragIn.normal));
	out_Color = 
	0.2*ambientLightColor 
	+ lightColor*(dotprod>0?dotprod:0.0f)
	+ (FragIn.height>0.8f?vec4(1,1,1,1):vec4(0))
	+ (FragIn.height<0.2f?vec4(0,0,1,1):vec4(0));

	out_Color.a = 1.0f;
	//out_Color = vec4(normalize((FragIn.normal)), 1.0f);
	//out_Color = vec4(FragIn.normal, 1.0f);
	//out_Color = vec4(1.0f);
}