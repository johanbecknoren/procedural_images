#version 330

layout(location = 0) in  vec3  in_Position;
layout(location = 1) in  vec3  in_Normal;
layout(location = 2) in  vec2  in_texCoord;

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;

  return 130.0 * dot(m, g);
}

uniform mat4 mvp;
uniform vec3 camPos;
uniform uint gridWidth;
uniform uint gridHeight;
uniform float gridSpacing;

const float maxHeight = 1.5f;
float du = 1.f/float(gridWidth);
float dv = 1.f/float(gridHeight);

out VertexData {
	vec3 pos;
	vec3 normal;
    vec3 viewDir;
    float height;
    float depth;
} VertexOut;

float normalizeDepth(float depth)
{
	float far = 400.f;
	float near = 1.0f;
	return (far+near)/(far-near) + 1.f/depth*((-2.f*far*near)/(far-near));
}

float getGradU(vec2 samplePos)
{
	return snoise(vec2(samplePos.x+du, samplePos.y)) - snoise(vec2(samplePos.x-du, samplePos.y));
}

float getGradV(vec2 samplePos)
{
	return snoise(vec2(samplePos.x, samplePos.y+dv)) - snoise(vec2(samplePos.x, samplePos.y-dv));
}

vec3 getNormalFromGrad(float ugrad, float vgrad)
{
	return vec3(ugrad, vgrad, 0.01f);
	//return vec3(-ugrad, 0.1f, vgrad);
}

vec3 getNormalVector(vec2 samplePos)
{
	return getNormalFromGrad(getGradU(samplePos), getGradV(samplePos));
}

vec4 sumOctaves(vec2 samplePos, float initFreq, int numOctaves, float persistence)
{
	float amp = 1.f;
	float maxAmp = 0.f;
	float res = 0.f;
	float freq = initFreq;
	vec3 normal = vec3(0.f);
	vec4 comboRes;
	for(int i=0; i<numOctaves; ++i)
	{
		res += snoise(samplePos*freq) * amp;
		normal += getNormalVector(samplePos*freq) * amp;

		maxAmp += amp;
		amp *= persistence;
		freq *= 2.f;
	}

	res /= maxAmp;

	res = res * (1.f-0.f)/2.f + (1.f+0.f)/2.f;

	return vec4(normal.x, normal.y, normal.z, res);
}

void main(void)
{
	vec2 sample = vec2(in_Position.x, in_Position.z) + camPos.xz/float(gridSpacing);
	vec3 hmNorm = in_Normal;
	vec3 hmPos = in_Position;
	
	vec4 normalAndHeight = sumOctaves(sample, 1.f/1.5f, 4, 0.3f);
	hmNorm = normalAndHeight.rgb;
	hmPos.y =  normalAndHeight.a;
	hmPos.y *= maxHeight;

	if(hmPos.y < 0.45*maxHeight)
	{
	 	hmPos.y = 0.45*maxHeight;
	 	//hmNorm = vec3(0,1,0);
	}
	
	vec3 viewVec = camPos + hmPos;
	float depth = 1.f-normalizeDepth(length(viewVec));
	
	vec4 pos = mvp * vec4(hmPos, 1.0f);

	VertexOut.viewDir = normalize(-camPos - hmPos);
	VertexOut.normal = normalize(hmNorm);
	VertexOut.height = hmPos.y / maxHeight;
	VertexOut.depth = depth;

    VertexOut.pos.x = in_Position.x/gridWidth;
    VertexOut.pos.y = in_Position.y/gridHeight;
    VertexOut.pos.z = 0.0f;

    gl_Position = pos;
}
