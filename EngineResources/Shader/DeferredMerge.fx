struct VertexIn
{
    float4 Position : POSITION;
    float4 Texcoord : TEXTURECOORD;
};

struct VertexOut
{
    // 
    float4 Position : SV_POSITION;
    float4 Texcoord : TEXTURECOORD;
};

VertexOut DeferredMerge_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut) 0;
    Out.Position = _In.Position;
    Out.Texcoord = _In.Texcoord;
    return Out;
}


Texture2D DiffuseTex : register(t0);
Texture2D DiffuseLight : register(t1);
Texture2D SpacularLight : register(t2);
Texture2D AmbientLight : register(t3);

SamplerState Smp : register(s0);


float4 DeferredMerge_PS(VertexOut _In) : SV_Target0
{
    float4 vDiffuseColor = (DiffuseTex.Sample(Smp, _In.Texcoord.xy));
    float4 DiffuseLightColor = (DiffuseLight.Sample(Smp, _In.Texcoord.xy));;
    float4 SpacularLightColor = (SpacularLight.Sample(Smp, _In.Texcoord.xy));;
    float4 AmbientLightColor = (AmbientLight.Sample(Smp, _In.Texcoord.xy));;
    
    if (0 == vDiffuseColor.a)
    {
        clip(-1);
    }
    
        float4 Color = vDiffuseColor;
    Color *= (DiffuseLightColor + SpacularLightColor + AmbientLightColor);
    Color.w = 1.0f;

    
    return Color;
}


