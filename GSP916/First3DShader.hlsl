struct VSInput
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
};

struct VSOutput // = PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};



// VertexShader
VSOutput VShader(VSInput p_Input)
{
	VSOutput _Out;

	_Out.Position = float4(p_Input.Position, 1);
	_Out.Color = p_Input.Color;

	return _Out;
}



// PixelShader
float4 PShader(VSOutput p_Input) : SV_TARGET
{
	return p_Input.Color;
}