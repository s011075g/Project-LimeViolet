// Structures
struct Material
{
	float4 diffuseColor;
	float3 specularColor;
	float specularPower;
};

struct VS_OUTPUT
{
	float4 PosH : SV_POSITION;
	float4 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD0;
	float4 TanW : TANGENT;
};

//Constant Buffers
cbuffer PerDraw : register(b0)
{
	matrix gViewProjection;
}

cbuffer PerObject : register(b1)
{
	matrix gWorld;
	Material gMaterial;
}

//Textures
Texture2D TexDiffuse : register(t0);
Texture2D TexSpecular : register(t1);
Texture2D TexNormal : register(t2);
Texture2D TexOcclusion : register(t3);

SamplerState textureSampler : register(s0);

VS_OUTPUT VS(float4 PosL : POSITION, float2 Tex : TEXCOORD, float3 NormalL : NORMAL, float4 TangentL : TANGENT)
{
	PosL.w = 1;
	VS_OUTPUT result = (VS_OUTPUT)0;

	result.PosW = mul(PosL, gWorld);
	result.PosH = mul(result.PosW, gViewProjection);
	result.NormalW = normalize(mul(float4(NormalL, 0), gWorld)).xyz;
	result.TanW = float4(mul(float4(TangentL.xyz, 0), gWorld).xyz, TangentL.w);
	result.Tex = Tex;

	return result;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return float4(input.NormalW,1);
	//return TexDiffuse.Sample(textureSampler, input.Tex);
}