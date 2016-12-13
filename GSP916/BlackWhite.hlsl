struct VSInput
{
	float3 Position : POSITION0;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

VSOutput VShader(VSInput pInput)
{
	VSOutput _Out;

	_Out.Position = float4(pInput.Position, 1);

	_Out.UV = _Out.Position.xy * 0.5 + 0.5;
	_Out.UV.y = 1 - _Out.UV.y;

	return _Out;
}

Texture2D Rendertarget;
SamplerState Sampler;

float4 PShader(VSOutput pInput) : SV_TARGET
{
	float4 Color = Rendertarget.Sample(Sampler, pInput.UV).rgba;

	//float Intensity = (Color.r * 0.3f + Color.g * 0.59f + Color.b * 0.11f);

	float3 Intensity;
	Intensity.r = Color.rgb * float3(0.393f, 0.769f, 0.189f);
	Intensity.g = Color.rgb * float3(0.349f, 0.686f, 0.168f);
	Intensity.b = Color.rgb * float3(0.272f, 0.534f, 0.131f);

	// 1,0,0 = 0.3
	// 0,1,0 = 0.59
	// 0,0,1 = 0.11

	// 0.3, 0.59, 0.11


	return float4(Intensity.rgb, Color.a);
}



