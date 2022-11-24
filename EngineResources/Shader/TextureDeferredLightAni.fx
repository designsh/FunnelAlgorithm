#include "CbufferHeader.fx"
#include "LightHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
    float4 BiNormal : BINORMAL;
    float4 Tangent : TANGENT;
    float4 Weight : BLENDWEIGHT; 
    int4 Index : BLENDINDICES;
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 ViewNormal : NORMAL;
    float4 ViewBiNormal : BINORMAL;
    float4 ViewTangent : TANGENT;
};

VertexOut TextureDeferredLightAni_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;

    Skinning(_In.Position, _In.Weight, _In.Index, ArrAniMationMatrix);
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    Out.Texcoord = _In.Texcoord;
    
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;
    
    _In.BiNormal.w = 0.0f;
    Out.ViewBiNormal = normalize(mul(_In.BiNormal, WV_));
    Out.ViewBiNormal.w = 0.0f;
    
    _In.Tangent.w = 0.0f;
    Out.ViewTangent = normalize(mul(_In.Tangent, WV_));
    Out.ViewTangent.w = 0.0f;
    
    return Out;
}


struct DeferredOutPut
{
    float4 ViewDif : SV_Target0;
    float4 ViewPos : SV_Target1;
    float4 ViewNor : SV_Target2;
};

Texture2D DiffuseTex : register(t0);
Texture2D NormalTex : register(t1);
SamplerState Smp : register(s0);

DeferredOutPut TextureDeferredLightAni_PS(VertexOut _In)
{
    DeferredOutPut Out;

    Out.ViewDif = (DiffuseTex.Sample(Smp, _In.Texcoord.xy));
    Out.ViewPos = _In.ViewPosition;
    Out.ViewPos.w = 1.0f;
    
    // 범프 텍스처
    Out.ViewNor = _In.ViewNormal;
    Out.ViewNor.w = 1.0f;
 

        Out.ViewNor = BumpNormalCalculate(NormalTex, Smp, _In.Texcoord, _In.ViewTangent, _In.ViewBiNormal, _In.ViewNormal);
        Out.ViewNor.w = 1.0f;
    
    return Out;
}

