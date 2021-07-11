cbuffer CBTransform : register(b0)
{
    matrix WVP;
    matrix World;
    matrix WorldInverse;
};

struct VS_INPUT
{
    float3 Position     : POSITION;
    float3 Normal       : NORMAL;
    float2 TexCoord     : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float2 TexCoord     : TEXCOORD0;
    float4 Position     : SV_Position;
};

VS_OUTPUT main(VS_INPUT IN)
{
    VS_OUTPUT OUT;

    OUT.Position = mul(WVP, float4(IN.Position, 1.f));
    OUT.PositionWS = mul(World, float4(IN.Position, 1.f));
    OUT.NormalWS = mul((float3x3)WorldInverse, IN.Normal);

    OUT.TexCoord = IN.TexCoord;
    
    return OUT;
}