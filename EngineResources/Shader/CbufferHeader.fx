
cbuffer TransformData : register(b0)
{
    float4 vWorldPosition_;
    float4 vWorldRotation_;
    float4 vWorldScaling_;

    float4 vLocalPosition_;
    float4 vLocalRotation_;
    float4 vLocalScaling_;

    float4x4 LocalScaling_;
    float4x4 LocalRotation_;
    float4x4 LocalPosition_;
    float4x4 LocalWorld_;

    float4x4 Parent_;
    float4x4 WorldWorld_;

    float4x4 View_;
    float4x4 Projection_;

    float4x4 WV_;
    float4x4 WVP_;
};

cbuffer RendererData : register(b12)
{
    int IsBump;
    int Shadow;
    int IsAni;
    int Temp2;
    int Temp3;
    int Temp4;
    int Temp5;
    int Temp6;
};

// _TargetNormal 계산하기 원하는 노말
// _Tangent, _BiNormal, _Normal
// 변화되어서 리턴되는 함수입니다.

//Texture2D DiffuseTex : register(t0);
//SamplerState Smp : register(s0);
float4 BumpNormalCalculate(Texture2D NormalTex, SamplerState Smp, float4 UV, float4 _Tangent, float4 _BiNormal, float4 _Normal)
{
    float4 TexNormal = NormalTex.Sample(Smp, UV.xy);
    TexNormal = TexNormal * 2.0f - 1.0f;
    TexNormal.w = 0.0f;
    TexNormal = normalize(TexNormal);
    
    float4 Result = (float4) 0.0f;
    float3x3 TangentSpace = float3x3(_Tangent.xyz, _BiNormal.xyz, _Normal.xyz);
    Result.xyz = mul(TexNormal.xyz, TangentSpace);
    
    return Result;

}