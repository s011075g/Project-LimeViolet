#version 330
#define GLSL_VERSION 330
#include "common.glh"

#if defined(VS_BUILD)
layout(binding = 0) uniform PerDraw
{
	mat4 viewProjection;
} pd;

layout(binding = 1) uniform PerObject
{
	mat4 world;
	vec4 diffuseColor;
	vec3 specularColor;
	float specularPower;
} po;

Layout(0) in vec3 positionL;
Layout(1) in vec2 texCoord;
Layout(2) in vec3 normalL;
Layout(3) in vec4 tangentL;

Layout(0) out vec4 PosH;
Layout(1) out vec4 PosW;
Layout(2) out vec3 NormalW;
Layout(3) out vec2 TexCoord;
Layout(4) out vec4 TanW;

void main()
{
	PosW = vec4(positionL, 1.0) * po.world;
	PosH = PosW * pd.viewProjection;
    gl_Position = PosH;
	NormalW = normalize(vec4(normalL, 0.0) * po.world).xyz;
    texCoord0 = texCoord;
	TanW = vec4(vec4(tangentL.xyz, 0) * po.world).xyz), tangentL.w);
}

#elif defined(FS_BUILD)
uniform sampler2D diffuse;

Layout(location = 0) in vec4 PosH;
Layout(location = 1) in vec4 PosW;
Layout(location = 2) in vec3 NormalW;
Layout(location = 3) in vec2 TexCoord;
Layout(location = 4) in vec4 TanW;

DeclareFragOutput(0, vec4);
void main()
{
	SetFragOutput(0, vec4(NormalW, 1));
}
#endif