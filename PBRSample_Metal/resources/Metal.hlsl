#include"PBR.hlsli"

cbuffer Constants : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProjection;
    
    float4 gLightPosition;
    float4 gLightColor;
}

struct VertexInput
{
    float3 Position : POSITION0;
    float4 Normal : NORMAL0;
    float2 Texcoord : TEXCOORD0;
};

struct PixelInput
{
    float4 Position : SV_Position;
    float4 Normal : TEXCOORD0;
    float2 Texcoord : TEXCOORD1;
    float4 WorldPosition : TEXCOORD2;

};

PixelInput vert(VertexInput input)
{
    PixelInput output = (PixelInput) 0;
    
    output.Position = mul(float4(input.Position, 1.0f), gWorld);
    output.WorldPosition = output.Position;
    output.Position = mul(output.Position, gView);
    output.Position = mul(output.Position, gProjection);
    
    output.Normal = input.Normal;
    output.Texcoord = input.Texcoord;
    
	return output;
}

float4 frag(PixelInput input) : SV_Target0
{
    float4 color;
    float4 lightPos = gLightPosition;

    lightPos.w = 1.0f;
    
    float4 lightDir = lightPos - input.Normal;
    
    float intensity = gLightPosition.w;
    float diffuse = dot(lightDir, -input.Normal) * gLightColor;
    
    float fr =  Fresnel(lightDir, normalize(input.Normal), 0.5f, 0.25f);
    
    return fr;
}