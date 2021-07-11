#define MAX_LIGHTS 8

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct _Material
{
    float4  Emissive;
    float4  Ambient; 
    float4  Diffuse; 
    float4  Specular;
    float   SpecularPower;
    bool    UseTexture;
};

cbuffer CBBasicMaterial : register(b0)
{
    _Material Material;
};

struct CBLightProperties
{
    float4      Position;
    float4      Direction;
    float4      Color;
    float       SpotAngle;
    float       ConstantAttenuation;
    float       LinearAttenuation;
    float       QuadraticAttenuation;
    int         LightType;
    bool        IsEnabled;
};

cbuffer CBBasicLighting : register(b1)
{
    float4              CameraPosition;
    float4              GlobalAmbient;
    CBLightProperties   Lights[MAX_LIGHTS];
};

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

float4 DoDiffuse(CBLightProperties light, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    return light.Color * NdotL;
}

float4 DoSpecular(CBLightProperties light, float3 V, float3 L, float3 N)
{
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));
    return light.Color * pow(RdotV, Material.SpecularPower);
}

float DoAttenuation(CBLightProperties light, float d)
{
    return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

LightingResult DoPointLight(CBLightProperties light, float3 V, float4 P, float3 N)
{
    LightingResult result;

    float3 L = (light.Position - P).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);

    result.Diffuse = DoDiffuse(light, L, N) * attenuation;
    result.Specular = DoSpecular(light, V, L, N) * attenuation;

    return result;
}

LightingResult DoDirectionalLight(CBLightProperties light, float3 V, float4 P, float3 N)
{
    LightingResult result;

    float3 L = -light.Direction.xyz;

    result.Diffuse = DoDiffuse(light, L, N);
    result.Specular = DoSpecular(light, V, L, N);

    return result;
}

float DoSpotCone(CBLightProperties light, float3 L)
{
    float minCos = cos(light.SpotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    float cosAngle = dot(light.Direction.xyz, -L);
    return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(CBLightProperties light, float3 V, float4 P, float3 N)
{
    LightingResult result;

    float3 L = (light.Position - P).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation(light, distance);
    float spotIntensity = DoSpotCone(light, L);

    result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
    result.Specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;

    return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
    float3 V = normalize(CameraPosition - P).xyz;

    LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };

    [unroll]
    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        LightingResult result = { {0, 0, 0, 0}, {0, 0, 0, 0} };

        if (!Lights[i].IsEnabled) continue;

        switch (Lights[i].LightType)
        {
            case DIRECTIONAL_LIGHT:
            {
                result = DoDirectionalLight(Lights[i], V, P, N);
            }
            break;
            case POINT_LIGHT:
            {
                result = DoPointLight(Lights[i], V, P, N);
            }
            break;
            case SPOT_LIGHT:
            {
                result = DoSpotLight(Lights[i], V, P, N);
            }
            break;
        }

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);

    return totalResult;
}

struct PS_INPUT
{
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float2 TexCoord     : TEXCOORD0;
};

float4 main(PS_INPUT IN) : SV_TARGET
{
    LightingResult lit = ComputeLighting(IN.PositionWS, normalize(IN.NormalWS));

    float4 emissive = Material.Emissive;
    float4 ambient = Material.Ambient * GlobalAmbient;
    float4 diffuse = Material.Diffuse * lit.Diffuse;
    float4 specular = Material.Specular * lit.Specular;

    float4 texColor = { 1, 1, 1, 1 };

    if (Material.UseTexture)
    {
        texColor = Texture.Sample(Sampler, IN.TexCoord);
    }

    float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

    return finalColor;
}