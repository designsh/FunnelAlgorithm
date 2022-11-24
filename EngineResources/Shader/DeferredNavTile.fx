#include "CbufferHeader.fx"
#include "LightHeader.fx"

struct VertexIn
{
    float4 Position : POSITION;
    float4 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 Position : SV_POSITION;
    float4 ViewPosition : POSITION;
    float4 ViewNormal : NORMAL;
    float4 Color : COLOR;
};

VertexOut DeferredNavTile_VS(VertexIn _In)
{
    VertexOut Out = (VertexOut)0;
    
    // [][][][]
    // [][][][]
    // [][][][1]
    // [][][][]
    
    _In.Position.w = 1.0f;
    Out.Position = mul(_In.Position, WVP_);
    Out.ViewPosition = mul(_In.Position, WV_);
    _In.Normal.w = 0.0f;
    Out.ViewNormal = normalize(mul(_In.Normal, WV_));
    Out.ViewNormal.w = 0.0f;

    Out.Color = _In.Color;
    
    return Out;
}

cbuffer ResultColor : register(b0)
{
    float4 vDiffuseColor;
};

struct DeferredOutPut 
{
    float4 ViewDif : SV_Target0; // ÅØ½ºÃ³ »ö±ò
    float4 ViewPos : SV_Target1;
    float4 ViewNor : SV_Target2;
};

DeferredOutPut DeferredNavTile_PS(VertexOut _In)
{
    //DiffuseLight += CalculateDirectionDiffuseLight(_In.ViewNormal, Lights[i]);
    //SpacularLight += CalculateDirectionSpacularLight(_In.ViewPosition, _In.ViewNormal, Lights[i]);
    
    DeferredOutPut Out;
    
    Out.ViewDif = _In.Color;
    Out.ViewNor = _In.ViewNormal;
    Out.ViewPos = _In.ViewPosition;
    
    
    return Out;
}