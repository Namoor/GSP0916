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

	float4 Dir_LightDirectionRGB_SpecularIntensityA;
	float4 Dir_LightColorRGB_SpecularExponentA;

	float4 PL1_LightPositionRGB_RangeA;
	float4 PL1_LightColorRGB_SpecularExponentA;
	float4 PL1_SpecularIntensityA;

	float4 PL2_LightPositionRGB_RangeA;
	float4 PL2_LightColorRGB_SpecularExponentA;
	float4 PL2_SpecularIntensityA;

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

float3 PhongShading(float3 LightInDir, float3 Normal, float3 ViewOutDir, float3 SpecularColor, float SpecularIntensity, float SpecularExponent)
{
	//float3 ReflectedLight = reflect(LightInDir, Normal);
	float3 ReflectedLight = normalize(LightInDir - Normal * 2 * dot(LightInDir, Normal));

	float SpecularPower = saturate(dot(ReflectedLight, ViewOutDir));

	SpecularPower = pow(SpecularPower, SpecularExponent);


	return SpecularColor * SpecularPower  * SpecularIntensity;
}

float3 BlinnPhongShading(float3 LightInDir, float3 Normal, float3 ViewOutDir, float3 SpecularColor, float SpecularIntensity, float SpecularExponent)
{
	//float3 ReflectedLight = reflect(LightInDir, Normal);
	float3 HalfVec = normalize(( ViewOutDir - LightInDir) * 0.5f);

	float SpecularPower = saturate(dot(HalfVec, Normal));

	SpecularPower = pow(SpecularPower, SpecularExponent);

	return SpecularColor * SpecularPower * SpecularIntensity;
}

float4 PShader(VSOutput pInput) : SV_TARGET
{
	// Normal
	float3 NormalLocal = NormalMap.Sample(Sampler, pInput.TexCoords).xzy;
	NormalLocal = NormalLocal * 2 - 1;
	NormalLocal.z = -NormalLocal.z;

	float3 Normal = NormalLocal.x * pInput.Tangent + NormalLocal.y * pInput.Normal + NormalLocal.z * pInput.BiTangent;




	// Lighting
	float3 PixelToCamera = normalize(Cam_CameraPositionInWorldSpace - pInput.PositionInWorldSpace);
	
	
	// Directional Light
	float3 DirectionalLightDir = normalize(Dir_LightDirectionRGB_SpecularIntensityA.xyz);

	float DirectionalLightIntensity = dot(Normal, DirectionalLightDir);
	float3 DirectionalLightColor = saturate(Dir_LightColorRGB_SpecularExponentA.rgb * DirectionalLightIntensity);

	
	float3 DirectionalSpecularColor = BlinnPhongShading(-DirectionalLightDir, Normal, PixelToCamera, Dir_LightColorRGB_SpecularExponentA.rgb, Dir_LightDirectionRGB_SpecularIntensityA.a, Dir_LightColorRGB_SpecularExponentA.a);

	// PointLight1
	float3 PixelToPL1 = PL1_LightPositionRGB_RangeA.xyz - pInput.PositionInWorldSpace;
	float3 PixelToPL1Norm = normalize(PixelToPL1);
	float Distance1 = PixelToPL1.x / PixelToPL1Norm.x;

	float DistanceIntensity1 = saturate(1 - (Distance1 / PL1_LightPositionRGB_RangeA.a));

	// PL1 Diffuse
	float3 PL1Diffuse = DistanceIntensity1 * saturate(dot(PixelToPL1Norm, Normal)) * PL1_LightColorRGB_SpecularExponentA.rgb;

	// PL1 Specular
	float3 PL1Spec = DistanceIntensity1 * BlinnPhongShading(-PixelToPL1Norm, Normal, PixelToCamera, PL1_LightColorRGB_SpecularExponentA.rgb, PL1_SpecularIntensityA.a, PL1_LightColorRGB_SpecularExponentA.a);

	// PointLight2
	float3 PixelToPL2 = PL2_LightPositionRGB_RangeA.xyz - pInput.PositionInWorldSpace;
	float3 PixelToPL2Norm = normalize(PixelToPL2);
	float Distance2 = PixelToPL2.x / PixelToPL2Norm.x;

	float DistanceIntensity2 = saturate(1 - (Distance2 / PL2_LightPositionRGB_RangeA.a));

	// PL1 Diffuse
	float3 PL2Diffuse = DistanceIntensity2 * saturate(dot(PixelToPL2Norm, Normal)) * PL2_LightColorRGB_SpecularExponentA.rgb;

	// PL1 Specular
	float3 PL2Spec = DistanceIntensity2 * BlinnPhongShading(-PixelToPL2Norm, Normal, PixelToCamera, PL2_LightColorRGB_SpecularExponentA.rgb, PL2_SpecularIntensityA.a, PL2_LightColorRGB_SpecularExponentA.a);


	// Ambient Light
	float3 AmbientLightColor = Amb_LightColor.rgb;



	// Light Combine
	float3 LightIntensity = saturate(AmbientLightColor + DirectionalLightColor + DirectionalSpecularColor + PL1Diffuse + PL1Spec + PL2Diffuse + PL2Spec);


	// Texture Color
	float3 DiffuseColor = DiffuseMap.Sample(Sampler, pInput.TexCoords).rgb;

	//return float4(SpecularIntensity.xxx, 1);

	return float4(DiffuseColor * LightIntensity,1);
}