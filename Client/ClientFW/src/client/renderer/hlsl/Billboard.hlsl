#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __BILLBOARD_HLSL__
#define __BILLBOARD_HLSL__

void GetBillboardConrers(float3 position, float2 size, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(g_view._12_22_32, look));
    up = normalize(cross(look, right));
    
    vertices[0] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[1] = float4(position + size.x * right + size.y * up, 1.0f);
    vertices[2] = float4(position - size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position - size.x * right + size.y * up, 1.0f);
}

void GetFixUpBillboardConrers(float3 position, float2 size, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    up = float3(0.0f, 1.0f, 0.0f);
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(up, look));
    
    vertices[0] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[1] = float4(position + size.x * right + size.y * up, 1.0f);
    vertices[2] = float4(position - size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position - size.x * right + size.y * up, 1.0f);
}

static float2 s_billboard_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

//
// Texture Billbaord
//

struct VS_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    uint resource_index : RSINDEX;
    float2 offset : OFFSET;
    float2 tilling : TILLING;
};

struct GS_BILLBOARD_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    uint resource_index : RSINDEX;
    float2 uv : TEXCOORD;
};

VS_BILLBOARD_INPUT VSBillboard(VS_BILLBOARD_INPUT input)
{
    return input;
}

[maxvertexcount(4)]
void GSBillboard(point VS_BILLBOARD_INPUT input[1], inout TriangleStream<GS_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_BILLBOARD_OUTPUT output;
    
    output.resource_index = input[0].resource_index;
    output.normal = look;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i] * input[0].tilling + input[0].offset;
        
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSFixUpBillboard(point VS_BILLBOARD_INPUT input[1], inout TriangleStream<GS_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetFixUpBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_BILLBOARD_OUTPUT output;
    
    output.resource_index = input[0].resource_index;
    output.normal = look;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i] * input[0].tilling + input[0].offset;
        
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSBillboard(GS_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    float4 base_color = g_texture_data[input.resource_index].Sample(g_sampler_point_wrap, input.uv);
    
    clip(base_color.a - MASKED_ALPHA);
    output.base_color = base_color;
    output.base_color.a = 1.0f;
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return output;
}

//
// Material Billboard
//

struct VS_MAT_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    uint resource_index : RSINDEX;
};

VS_MAT_BILLBOARD_INPUT VSMaterialBillboard(VS_MAT_BILLBOARD_INPUT input)
{
    return input;
}

//
// Material BaseColor Billboard
//

struct GS_MAT_BASECOLOR_BILLBOARD_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    float4 base_color : COLOR;
    float roughness : ROUGHNESS;
    float metallic : METALLIC;
};

[maxvertexcount(4)]
void GSMaterialBaseColorBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_BASECOLOR_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_BASECOLOR_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.base_color = material_data.base_color;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    
   [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSFixUpMaterialBaseColorBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_BASECOLOR_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetFixUpBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_BASECOLOR_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.base_color = material_data.base_color;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSOpaqueMaterialBaseColorBillboard(GS_MAT_BASECOLOR_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = input.base_color;
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

PS_GBUFFER_OUTPUT PSMaskedMaterialBaseColorBillboard(GS_MAT_BASECOLOR_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = input.base_color;
    clip(output.base_color.a - MASKED_ALPHA);
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Material Texture Billboard
//

struct GS_MAT_TEXTURE_BILLBOARD_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    uint diffuse_index : DIFFUSE_TEXTURE;
    float roughness : ROUGHNESS;
    float metallic : METALLIC;
    float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void GSMaterialTextureBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_TEXTURE_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_TEXTURE_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.diffuse_index = material_data.diffuse_texture_index;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    
   [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i];
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSFixUpMaterialTextureBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_TEXTURE_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetFixUpBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_TEXTURE_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.diffuse_index = material_data.diffuse_texture_index;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i];
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSOpaqueMaterialTextureBillboard(GS_MAT_TEXTURE_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = g_texture_data[input.diffuse_index].Sample(g_sampler_point_wrap, input.uv);
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

PS_GBUFFER_OUTPUT PSMaskedMaterialTextureBillboard(GS_MAT_TEXTURE_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = g_texture_data[input.diffuse_index].Sample(g_sampler_point_wrap, input.uv);
    clip(output.base_color.a - MASKED_ALPHA);
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Material Normal Map Billboard
//

struct GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    uint diffuse_index : DIFFUSE_TEXTURE;
    uint normal_index : NORMAL_TEXTURE;
    float roughness : ROUGHNESS;
    float metallic : METALLIC;
    float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void GSMaterialNormalMapBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.diffuse_index = material_data.diffuse_texture_index;
    output.normal_index = material_data.normal_texture_index;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    output.tangent = right;
    output.bitangent = up;
    
   [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i];
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSFixUpMaterialNormalMapBillboard(point VS_MAT_BILLBOARD_INPUT input[1], inout TriangleStream<GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetFixUpBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT output;
    
    MaterialData material_data = g_material_data[input[0].resource_index];
    
    output.diffuse_index = material_data.diffuse_texture_index;
    output.normal_index = material_data.normal_texture_index;
    output.roughness = material_data.roughness;
    output.metallic = material_data.metallic;
    output.normal = look;
    output.tangent = right;
    output.bitangent = up;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_billboard_uvs[i];
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSOpaqueMaterialNormalMapBillboard(GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = g_texture_data[input.diffuse_index].Sample(g_sampler_point_wrap, input.uv);
    float3 normal = GetNormalFromNormalMap(g_texture_data[input.normal_index].Sample(g_sampler_point_wrap, input.uv).xyz,
        input.normal, input.tangent, input.bitangent);
    output.normal = float4(normal + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

PS_GBUFFER_OUTPUT PSMaskedMaterialNormalMapBillboard(GS_MAT_NORMAL_MAP_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
  
    output.base_color = g_texture_data[input.diffuse_index].Sample(g_sampler_point_wrap, input.uv);
    clip(output.base_color.a - MASKED_ALPHA);
    
    float3 normal = GetNormalFromNormalMap(g_texture_data[input.normal_index].Sample(g_sampler_point_wrap, input.uv).xyz,
        input.normal, input.tangent, input.bitangent);
    output.normal = float4(normal + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(input.roughness, input.metallic, 1.0f, 1.0f);
    
    return output;
}

#endif // __BILLBOARD_HLSL__