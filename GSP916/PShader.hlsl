struct VertexShaderOutput // PixelShaderInput
{
	float4 Position : POSITION0;
	float3 Color : COLOR0;
};



float4 PShader(VertexShaderOutput p_Input) : SV_TARGET
{
	return float4(p_Input.Color.rgb, 1.0f);
}