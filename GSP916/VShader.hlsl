struct VertexShaderInput
{
	float3 Position : POSITION0;
	float3 Color : COLOR0;
};

struct VertexShaderOutput // PixelShaderInput
{
	float4 Position : SV_POSITION;
	float3 Color : COLOR0;
};


VertexShaderOutput VShader(VertexShaderInput p_Input)
{
	VertexShaderOutput _Out;

	_Out.Position = float4(p_Input.Position, 1);
	_Out.Color = p_Input.Color;

	return _Out;
}