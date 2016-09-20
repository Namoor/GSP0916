struct VertexShaderInput
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
};

struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};


VertexShaderOutput VShader(VertexShaderInput p_Input)
{
	VertexShaderOutput _Out;

	_Out.Position = float4(p_Input.Position, 1);
	_Out.Color = p_Input.Color;

	return _Out;
}


float4 PShader(VertexShaderOutput p_Input) : SV_TARGET
{
	return float4(p_Input.Color.rgb, 1.0f);
}