struct VSInput
{
	float3 Position : POSITION0; // ObjectSpace
	float3 Normal : NORMAL0;
	// COLOR
	// NORMAL
	// TANGENT
	// TEXCOORD
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Normal : NORMAL0;
};

cbuffer MatrixBuffer
{
	float4x4 MVP;
};


VSOutput VShader(VSInput pInput)
{
	VSOutput _Out;

	_Out.Position = mul(MVP, float4(pInput.Position, 1));
	_Out.Normal = float4(pInput.Normal, 0);

	return _Out;
}

float4 PShader(VSOutput pInput) : SV_TARGET
{
	float LightIntensity = dot(pInput.Normal.xyz , float3(0,1,0));

	return float4(LightIntensity, LightIntensity, LightIntensity,1);
}