struct VSInput
{
	float3 Position : POSITION0; // ObjectSpace

	float3 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 BiTangent : TANGENT1;

	float2 TexCoords : TEXCOORD0;
	// COLOR
	// NORMAL
	// TANGENT
	// TEXCOORD
};

struct VSOutput
{
	float4 Position : SV_POSITION;

	float4 PositionInWorldSpace : POSITION0;

	float4 Normal : NORMAL0;
	float3 Tangent : TANGENT0;
	float3 BiTangent : TANGENT1;

	float2 TexCoords : TEXCOORD0;
};

cbuffer MatrixBuffer // CB VS0
{
	float4x4 MVP;
};

cbuffer LightBuffer // CB PS0
{
	float4 Cam_CameraPositionInWorldSpace;

	float4 Dir_LightDirection;
	float4 Dir_LightColor;

	float4 Amb_LightColor;
};

Texture2D NormalMap;
Texture2D DiffuseMap; // T PS0

SamplerState Sampler;


VSOutput VShader(VSInput pInput)
{
	VSOutput _Out;

	_Out.Position = mul(MVP, float4(pInput.Position, 1));

	_Out.Normal = float4(pInput.Normal, 0);
	_Out.Tangent = float4(pInput.Tangent, 0);
	_Out.BiTangent = float4(pInput.BiTangent, 0);
	_Out.PositionInWorldSpace = float4(pInput.Position, 1);

	_Out.TexCoords = pInput.TexCoords;

	return _Out;
}

float4 PShader(VSOutput pInput) : SV_TARGET
{
	// Normal
	float3 NormalLocal = NormalMap.Sample(Sampler, pInput.TexCoords).xzy;
	NormalLocal = NormalLocal * 2 - 1;
	NormalLocal.z = -NormalLocal.z;

	float3 Normal = NormalLocal.x * pInput.Tangent + NormalLocal.y * pInput.Normal + NormalLocal.z * pInput.BiTangent;




	// Lighting
	// Directional Light
	float3 DirectionalLightDir = normalize(Dir_LightDirection.xyz);

	float DirectionalLightIntensity = dot(Normal, DirectionalLightDir);
	float3 DirectionalLightColor = saturate(Dir_LightColor * DirectionalLightIntensity);

	float3 PixelToCamera = normalize(Cam_CameraPositionInWorldSpace - pInput.PositionInWorldSpace);
	
	//float _Dot = dot(Normal, DirectionalLightDir);

	float3 PixelToCameraReflected = reflect(-DirectionalLightDir, Normal);// normalize(-DirectionalLightDir + Normal * 2 * _Dot);

	float SpecularIntensity = saturate(dot(PixelToCameraReflected, PixelToCamera));

	SpecularIntensity = saturate(pow(SpecularIntensity, 100) * 5);

	// Ambient Light
	float3 AmbientLightColor = Amb_LightColor.rgb;



	// Light Combine
	float3 LightIntensity = saturate(AmbientLightColor + DirectionalLightColor + SpecularIntensity);


	// Texture Color
	float3 DiffuseColor = DiffuseMap.Sample(Sampler, pInput.TexCoords).rgb;

	//return float4(SpecularIntensity.xxx, 1);

	return float4(DiffuseColor * LightIntensity,1);
}