#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __OPAQUE_HLSL__
#define __OPAQUE_HLSL__

//
// Opaque Material Mesh
//

struct VS_OPAQUE_MATERIAL_MESH_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_OPAQUE_MATERIAL_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
};

VS_OPAQUE_MATERIAL_MESH_OUT VSOpaqueMaterialMesh(VS_OPAQUE_MATERIAL_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_MATERIAL_MESH_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.sv_position = mul(position, g_view_projection);
    output.normal = normalize(mul(input.normal, (float3x3) i_data.world_inverse_transpose));
    
    return output;
}

[earlydepthstencil]
PS_GBUFFER_OUTPUT PSOpaqueMaterialMesh(VS_OPAQUE_MATERIAL_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    
    output.base_color = material_data.base_color;
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(material_data.roughness, material_data.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Opaque Texture Mesh
//

struct VS_OPAQUE_TEXTURE_MESH_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OPAQUE_TEXTURE_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OPAQUE_TEXTURE_MESH_OUT VSOpaqueTextureMesh(VS_OPAQUE_TEXTURE_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_TEXTURE_MESH_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.sv_position = mul(position, g_view_projection);
    output.normal = normalize(mul(input.normal, (float3x3) i_data.world_inverse_transpose));
    output.uv = input.uv;
    
    return output;
}

[earlydepthstencil]
PS_GBUFFER_OUTPUT PSOpaqueTextureMesh(VS_OPAQUE_TEXTURE_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    
    output.base_color = g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(material_data.roughness, material_data.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Opaque Normal Map Mesh
//

struct VS_OPAQUE_NORMAL_MAP_MESH_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VS_OPAQUE_NORMAL_MAP_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

VS_OPAQUE_NORMAL_MAP_MESH_OUT VSOpaqueNormalMapMesh(VS_OPAQUE_NORMAL_MAP_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_NORMAL_MAP_MESH_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.sv_position = mul(position, g_view_projection);
    output.normal = normalize(mul(input.normal, (float3x3) i_data.world_inverse_transpose));
    output.tangent = normalize(mul(input.tangent, (float3x3) i_data.world_inverse_transpose));
    output.bitangent = normalize(mul(input.bitangent, (float3x3) i_data.world_inverse_transpose));
    output.uv = input.uv;
    
    return output;
}

[earlydepthstencil]
PS_GBUFFER_OUTPUT PSOpaqueNormalMapMesh(VS_OPAQUE_NORMAL_MAP_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    output.base_color = g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
    
    float3 normal = GetNormalFromNormalMap(g_texture_data[material_data.normal_texture_index].Sample(g_sampler_point_wrap, input.uv).xyz,
        input.normal, input.tangent, input.bitangent);
    output.normal = float4(normal + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(material_data.roughness, material_data.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Opaque Mesh For Shadow
//

#include "shadow.hlsl"

struct VS_OPAQUE_MESH_FOR_SHADOW_IN
{
    float3 position : POSITION;
};

VS_SHADOW_OUT VSOpaqueMeshForShadow(VS_OPAQUE_MESH_FOR_SHADOW_IN input, uint instance_id : SV_InstanceID)
{
    VS_SHADOW_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.sv_position = mul(position, g_shadow_camera_data[0].g_view_projection);
    
    return output;
}

//
// Opaque Mesh For Cube Shadow
//

float4 VSOpaqueMeshForShadowCube(VS_OPAQUE_MESH_FOR_SHADOW_IN input, uint instance_id : SV_InstanceID) : SV_POSITION
{
    InstanceData i_data = g_instance_data[instance_id];
    
    return mul(float4(input.position, 1.0f), i_data.world);
}

[maxvertexcount(18)]
void GSOpaqueMeshForShadowCube(triangle float4 input[3] : SV_POSITION, inout TriangleStream<GS_SHADOW_CUBE_OUT> out_stream)
{
    GS_SHADOW_CUBE_OUT output;
    
    [unroll]
    for (int face = 0; face < 6; ++face)
    {
        output.render_target_index = face;
        
        [unroll]
        for (int i = 0; i < 3; ++i)
        {
            output.sv_position = mul(input[i], g_shadow_camera_data[face].g_view_projection);
            out_stream.Append(output);
        }

        out_stream.RestartStrip();
    }
}

//
// Opaque Mesh For Cascade Shadow
//

float4 VSOpaqueMeshForShadowCascade(VS_OPAQUE_MESH_FOR_SHADOW_IN input, uint instance_id : SV_InstanceID) : SV_POSITION
{
    InstanceData i_data = g_instance_data[instance_id];
    
    return mul(float4(input.position, 1.0f), i_data.world);
}

[maxvertexcount(9)]
void GSOpaqueMeshForShadowCascade(triangle float4 input[3] : SV_POSITION, inout TriangleStream<GS_SHADOW_CASCADE_OUT> out_stream)
{
    GS_SHADOW_CASCADE_OUT output;
    
    [unroll]
    for (int face = 0; face < 3; ++face)
    {
        output.render_target_index = face;
        
        [unroll]
        for (int i = 0; i < 3; ++i)
        {
            output.sv_position = mul(input[i], g_shadow_camera_data[face].g_view_projection);
            out_stream.Append(output);
        }

        out_stream.RestartStrip();
    }
}

#endif // __OPAQUE_HLSL__