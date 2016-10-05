struct VertexShaderInput
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};


VertexShaderOutput VShader(VertexShaderInput p_Input)
{
	VertexShaderOutput _Out;

	_Out.Position = float4(p_Input.Position, 1);
	_Out.Color = p_Input.Color;
	_Out.UV = p_Input.UV;

	return _Out;
}

Texture2D Tex0;
SamplerState Sampler;

float4 PShader(VertexShaderOutput p_Input) : SV_TARGET
{
	return float4(p_Input.Color.rgba * Tex0.Sample(Sampler, p_Input.UV).rgba);
}