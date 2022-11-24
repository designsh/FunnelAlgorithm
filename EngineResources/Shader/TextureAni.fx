#include "CbufferHeader.fx"
#include "AniHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
    float4 Weight : BLENDWEIGHT; // ���� ���������� ������ �󸶳� ������ �޴��Ŀ� ���� ����.
    int4 Index : BLENDINDICES; // ���� ���������� ���� �ε��� ����
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
    float4 Normal : NORMAL;
};

VertexOut TextureAni_VS(VertexIn _In) 
{
    VertexOut Out;

    if (0 != IsAni)
    {
        Skinning(_In.Position, _In.Weight, _In.Index, ArrAniMationMatrix);
    }
    
    Out.Position = _In.Position;

    Out.Position.w = 1.0f;
    Out.Position = mul(Out.Position, WVP_);
    Out.Texcoord = _In.Texcoord;
    return Out;
}


Texture2D DiffuseTex : register(t0);
SamplerState Smp : register(s0);

float4 TextureAni_PS(VertexOut _in) : SV_Target0
{
    float4 Color = (DiffuseTex.Sample(Smp, _in.Texcoord.xy));

    if (0.0f == Color.a)
    {
        // ��¾��ϰ� ����
        clip(-1);
    }

    return Color;
}


