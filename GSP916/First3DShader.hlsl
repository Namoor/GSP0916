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


cbuffer MatrixBuffer
{
	float4x4 RotationsMatrix;
};


// VertexShader
VSOutput VShader(VSInput p_Input)
{
	VSOutput _Out;

	_Out.Position = mul(RotationsMatrix, float4(p_Input.Position, 1));
	_Out.Color = p_Input.Color;

	return _Out;
}



// PixelShader
float4 PShader(VSOutput p_Input) : SV_TARGET
{
	return p_Input.Color;
}