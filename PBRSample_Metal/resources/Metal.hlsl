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
    
    output.Normal = float4(mul(input.Normal.xyz, (float3x3) gWorld), 1.0f);
    output.Texcoord = input.Texcoord;
    
	return output;
}

float evaluate(float4 wi, float4 wo, float4 normal, float fr)
{
    float coso = dot(wo, normal);
    float cosi = dot(wi, normal);
    
    if (cosi == 0.0f || coso == 0.0f)
    {
        return 0.0f;
    }
    
    float4 wh = normalize(wi + wo);
    
    float cosh = dot(wi, wh);
    
    float denom = 4.0f * coso * cosi;

    float numer = dot(wh, normal) * fr;
    
    return numer / denom;
}

float4 frag(PixelInput input) : SV_Target0
{
    float4 color;
    float4 lightPos = gLightPosition;
    float4 normal = normalize(input.Normal);
    lightPos.w = 1.0f;
    
    float4 lightDir = lightPos - input.Normal;
    
    float intensity = gLightPosition.w;
    float diffuse = saturate(dot(-lightDir, -normal) * gLightColor * intensity);
    
    
    float4 ref = reflect(normalize(lightDir), -normal);
    
    float4 wh = normalize(lightDir + ref);
    float fr = Fresnel(wh, -normal, 0.0f, 0.1f);
    
    float metallic = evaluate(normalize(lightDir), normalize(ref), -normal, fr);
    return metallic;
}