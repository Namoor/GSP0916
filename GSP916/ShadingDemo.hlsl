struct VSInput
{
	float3 Position : POSITION0; // ObjectSpace
	float3 Normal : NORMAL0;
	float2 TexCoords : TEXCOORD0;
	// COLOR
	// NORMAL
	// TANGENT
	// TEXCOORD
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Normal : NORMAL0;
	float2 TexCoords : TEXCOORD0;
};

cbuffer MatrixBuffer // CB VS0
{
	float4x4 MVP;
};

cbuffer LightBuffer // CB PS0
{
	float4 LightDirection;
	float4 LightColor;
};

Texture2D NormalMap;
Texture2D DiffuseMap; // T PS0

SamplerState Sampler;


VSOutput VShader(VSInput pInput)
{
	VSOutput _Out;

	_Out.Position = mul(MVP, float4(pInput.Position, 1));
	_Out.Normal = float4(pInput.Normal, 0);

	_Out.TexCoords = pInput.TexCoords;

	return _Out;
}

float4 PShader(VSOutput pInput) : SV_TARGET
{
	// Normal

	float3 Normal = NormalMap.Sample(Sampler, pInput.TexCoords).xzy;
	
	Normal = Normal * 2 - 1;
	Normal.z = - Normal.z;


	// Lighting
	float DirectionalLightIntensity = dot(Normal, normalize(LightDirection.xyz));
	float AmbientLightIntensity = 0.2f;

	float LightIntensity = saturate(AmbientLightIntensity + DirectionalLightIntensity);

	float3 DiffuseColor = DiffuseMap.Sample(Sampler, pInput.TexCoords).rgb;

	return float4(DiffuseColor * LightIntensity,1);
}