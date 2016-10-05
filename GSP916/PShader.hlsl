struct VertexShaderOutput // PixelShaderInput
{
	float4 Position : POSITION0;
	float3 Color : COLOR0;
};

cbuffer FirstCBuffer
{
	float4 Color;
}

Texture2D FirstTexture; // texture 0
SamplerState TextureSampler;

// Texture2D SecondTexture; // texture 1


float4 PShader(VertexShaderOutput p_Input) : SV_TARGET
{
	float2 _UV = p_Input.Color.rg;

	float4 _TextureColor = FirstTexture.Sample(TextureSampler, _UV);

	//return float4(p_Input.Color.rg, 0,1);
	return float4(Color.rgb * _TextureColor.rgb, 1);

	//return float4(Color.rgb * p_Input.Color.rgb, 1.0f);
}