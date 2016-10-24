struct VSInput
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct VSOutput // = PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
};


cbuffer MatrixBuffer
{
	float4x4 WVPMatrix;
	float4x4 WMatrix;
};


// VertexShader
VSOutput VShader(VSInput p_Input)
{
	VSOutput _Out;

	_Out.Position = mul(WVPMatrix, float4(p_Input.Position, 1));
	_Out.Color = p_Input.Color;
	_Out.UV = p_Input.UV;
	_Out.Normal = mul(WMatrix, float4( p_Input.Normal, 0));

	return _Out;
}

Texture2D BaseTexture;
sampler TextureSampler;

// PixelShader
float4 PShader(VSOutput p_Input) : SV_TARGET
{
	float3 _Normal = normalize(p_Input.Normal);

	// LightIntensity == cos(eingeschlossener Winkel)
	float LightIntensity = dot(_Normal, normalize(float3(3,2,1)));
	
	LightIntensity = saturate(LightIntensity);

	float FinalLightIntensity = LightIntensity + 0.2f;

	float4 Color = p_Input.Color * BaseTexture.Sample( TextureSampler, p_Input.UV );

	return float4(Color.rgb * FinalLightIntensity, Color.a);
}