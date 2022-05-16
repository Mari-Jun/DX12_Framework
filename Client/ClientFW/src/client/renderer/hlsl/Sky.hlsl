#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __WIDGET_HLSL__
#define __WIDGET_HLSL__

struct VS_SKYCUBE_INPUT
{
    float3 position : POSITION;
};

struct VS_SKYCUBE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
};

VS_SKYCUBE_OUTPUT VSSkyCube(VS_SKYCUBE_INPUT input)
{
    VS_SKYCUBE_OUTPUT output;
    
    output.position = input.position;
    
    float4 pos = float4(input.position, 1.0f);
    pos.xyz += g_camera_pos;
    
    output.sv_position = mul(pos, g_view_projection).xyww;
    
    return output;
}

PS_GBUFFER_OUTPUT PSSkyCube(VS_SKYCUBE_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;

    output.base_color = float4(g_texture_cube_data[g_material_index].Sample(g_sampler_point_wrap, input.position).xyz, 0.0f);
    output.normal = 0.0f;
    output.additional_info = 0.0f;
    
    return output;
}

struct VS_SKYSPHERE_INPUT
{
    float3 center_color : CENTERCOLOR;
    float3 apex_color : APEXCOLOR;
};

struct VS_SKYSPHERE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 center_color : CENTERCOLOR;
    float3 apex_color : APEXCOLOR;
};

struct HS_SKYSPHERE_CONSTANT_DATA_OUTPUT
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HS_SKYSPHERE_OUTPUT
{
    float3 position : POSITION;
    float3 center_color : CENTERCOLOR;
    float3 apex_color : APEXCOLOR;
};

struct DS_SKYSPHERE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
    float3 color : COLOR;
};

VS_SKYSPHERE_OUTPUT VSSkySphere(VS_SKYSPHERE_INPUT input)
{
    VS_SKYSPHERE_OUTPUT output;
    
    output.sv_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.center_color = input.center_color;
    output.apex_color = input.apex_color;
    
    return output;
}

HS_SKYSPHERE_CONSTANT_DATA_OUTPUT ConstantHSSkySphere()
{
    HS_SKYSPHERE_CONSTANT_DATA_OUTPUT output;
    
    float tess_factor = 24.0f;
    output.edges[0] = output.edges[1] =
    output.edges[2] = output.edges[3] = tess_factor;
    output.inside[0] = output.inside[1] = tess_factor;
    
    return output;
}

static float3 hemil_dir[2] = { float3(1.0f, 1.0f, 1.0f), float3(-1.0f, 1.0f, -1.0f) };

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHSSkySphere")]
HS_SKYSPHERE_OUTPUT HSSkySphere(InputPatch<VS_SKYSPHERE_OUTPUT, 1> input, uint patch_id : SV_PrimitiveID)
{
    HS_SKYSPHERE_OUTPUT output;
    
    output.position = hemil_dir[patch_id];
    output.center_color = input[0].center_color;
    output.apex_color = input[0].apex_color;
    
    return output;
}

[domain("quad")]
DS_SKYSPHERE_OUTPUT DSSkySphere(HS_SKYSPHERE_CONSTANT_DATA_OUTPUT input, float2 uv : SV_DomainLocation, OutputPatch<HS_SKYSPHERE_OUTPUT, 4> quad)
{
    float2 clip_space_uv = uv.xy * 2.0f - 1.0f;
    
    float2 clip_space_abs_uv = abs(clip_space_uv.xy);
    float max_length = max(clip_space_abs_uv.x, clip_space_abs_uv.y);
    
    float3 position = normalize(float3(clip_space_uv, (max_length - 1.0f)) * quad[0].position);
    
    DS_SKYSPHERE_OUTPUT output;
    
    float4 pos = float4(position, 1.0f);
    pos.xyz += g_camera_pos;
    
    output.sv_position = mul(pos, g_view_projection).xyww;
    output.position = position;
    output.color = lerp(quad[0].center_color, quad[0].apex_color, clamp(position.y, 0.0f, 1.0f));
    
    return output;
}

PS_GBUFFER_OUTPUT PSSkySphere(DS_SKYSPHERE_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;

    output.base_color = float4(input.color, 0.0f);
    output.normal = 0.0f;
    output.additional_info = 0.0f;
    
    return output;
}

#endif // __WIDGET_HLSL__