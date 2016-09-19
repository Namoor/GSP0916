struct VertexShaderOutput // PixelShaderInput
{
	float4 Position : POSITION0;
	float3 Color : COLOR0;
};

cbuffer FirstCBuffer
{
	float4 Color;
}


float4 PShader(VertexShaderOutput p_Input) : SV_TARGET
{
	return float4(Color.rgb * p_Input.Color.rgb, 1.0f);
}