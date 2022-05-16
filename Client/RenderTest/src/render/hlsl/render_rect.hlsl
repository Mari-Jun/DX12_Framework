
#include "../../../../ClientFW/src/client/renderer/hlsl/opaque.hlsl"

struct VS_DIFFUSE_IN
{
    float3 position : POSITION;
};

struct VS_DIFFUSE_OUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

VS_DIFFUSE_OUT VSDiffuse(VS_DIFFUSE_IN input, uint instance_id : SV_InstanceID)
{
    VS_DIFFUSE_OUT output;
    output.position = mul(float4(input.position, 1.0f), g_instance_data[instance_id].world);
    output.position.z = instance_id / 100.0f + 0.01f;
    
    float f = instance_id % 20.0f / 40.0f + 0.5f;
    float r = instance_id % 3 == 0 ? f : 0.0f;
    float g = instance_id % 3 == 1 ? f : 0.0f;
    float b = instance_id % 3 == 2 ? f : 0.0f;
    output.color = float4(r, g, b, 1.0f);
    
    return output;
}

float4 PSDiffuse(VS_DIFFUSE_OUT input) : SV_TARGET
{
    return input.color;
}