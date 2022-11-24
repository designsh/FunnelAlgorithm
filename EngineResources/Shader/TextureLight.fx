#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 ViewNormal : NORMAL;
};

VertexOut TextureLight_VS(VertexIn _In) 
{
    VertexOut Out = (VertexOut) 0;
    
    // [][][][]
    // [][][][]
    // [][][][1]
    // [][][][]
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    Out.Texcoord = _In.Texcoord;

    // w에 z값 들어있잖아.
    // Out.Position.w = 1.0f;
    
    
    // 픽셀을 건져내기 위한 포지션
    
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;
    return Out;
}


Texture2D DiffuseTex : register(t0);
SamplerState Smp : register(s0);

float4 TextureLight_PS(VertexOut _In) : SV_Target0
{
    float4 vDiffuseColor = (DiffuseTex.Sample(Smp, _In.Texcoord.xy));

    //if (0.0f == vDiffuseColor.a)
    //{
    //    // 출력안하고 정지
    //    clip(-1);
    //}

    float4 DiffuseLight = (float) 0.0f;
    float4 SpacularLight = (float) 0.0f;
    float4 AmbientLight = (float) 0.0f;
    for (int i = 0; i < LightCount; ++i)
    {
        DiffuseLight += CalculateDirectionDiffuseLight(_In.ViewNormal, Lights[i]);
        SpacularLight += CalculateDirectionSpacularLight(_In.ViewPosition, _In.ViewNormal, Lights[i]);
        AmbientLight += CalculateDirectionAmbientLight(Lights[i]);
    }
    
    float4 Color = vDiffuseColor;
    
    if (LightCount != 0)
    {
        Color *= (DiffuseLight + SpacularLight + AmbientLight);
    }
    
    Color.w = 1.0f;

    
    return Color;
}


