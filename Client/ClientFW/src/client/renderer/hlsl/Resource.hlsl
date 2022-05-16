#ifndef __RESOURCE_HLSL__
#define __RESOURCE_HLSL__

static const float MASKED_ALPHA = 0.333333f;
static const float PI = 3.14159265359;

struct InstanceData
{
    matrix world;
    matrix world_inverse_transpose;
    uint additional_info;
};

struct MaterialData
{
    float4 base_color;
    float roughness;
    float metallic;
    int diffuse_texture_index;
    int normal_texture_index;
    int roughness_texture_index;
    int metallic_texture_index;
};

struct LightData
{
    float3 light_color;
    float attenuation_radius;
    float3 light_direction;
    float cone_inner_angle;
    float3 light_position;
    float cone_outer_angle;
    int use_shadow;
    int is_static_light;
    uint shadow_texture_data_index;
};

struct ShadowCameraData
{
    matrix g_view_projection;
};

struct ShadowTextureData
{
    matrix uv_from_ndc;
    float2 inverse_texture_size;
    uint shadow_texture_index;
};

struct CascadeShadowTextureData
{
    float4 cascade_offset_x;
    float4 cascade_offset_y;
    float4 cascade_scale;
};

struct SkeletalData
{
    matrix bone_transform;
};

StructuredBuffer<InstanceData> g_instance_data : register(t0, space0);
StructuredBuffer<MaterialData> g_material_data : register(t1, space0);
StructuredBuffer<LightData> g_light_data : register(t2, space0);
StructuredBuffer<ShadowCameraData> g_shadow_camera_data : register(t3, space0);
StructuredBuffer<ShadowTextureData> g_shadow_texture_data : register(t4, space0);
StructuredBuffer<CascadeShadowTextureData> g_cascade_shadow_texture_data : register(t5, space0);
StructuredBuffer<SkeletalData> g_bone_transform_data : register(t6, space0);
Texture2D g_texture_data[] : register(t0, space1);
TextureCube g_texture_cube_data[] : register(t0, space2);
Texture2DArray g_texture_array_data[] : register(t0, space3);

SamplerState g_sampler_point_wrap : register(s0, space0);
SamplerComparisonState g_sampler_comparison_pcf_shadow : register(s6, space0);

cbuffer cbMaterialIndexData : register(b0, space0)
{
    uint g_material_index;
};

cbuffer cbRenderCameraData : register(b1, space0)
{
    matrix g_view;
    matrix g_projection;
    matrix g_view_projection;
    matrix g_inverse_view;
    float4 g_perspective_values;
    float3 g_camera_pos;
    uint g_final_texture_index;
    uint4 g_gbuffer_texture_indices;
    uint g_num_of_directional_light;
    uint g_render_camera_index;
}

#endif // __RESOURCE_HLSL__