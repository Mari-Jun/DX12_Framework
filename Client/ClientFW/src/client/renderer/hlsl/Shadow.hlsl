#include "resource.hlsl"

#ifndef __SHADOW_HLSL__
#define __SHADOW_HLSL__

struct VS_SHADOW_OUT
{
    float4 sv_position : SV_POSITION;
};

struct GS_SHADOW_CUBE_OUT
{
    float4 sv_position : SV_POSITION;
    uint render_target_index : SV_RENDERTARGETARRAYINDEX;
};

struct GS_SHADOW_CASCADE_OUT
{
    float4 sv_position : SV_POSITION;
    uint render_target_index : SV_RENDERTARGETARRAYINDEX;
};

float CalcShadowFactor(float3 position, bool is_static_light, ShadowTextureData shadow_data)
{
    float4 shadow_pos = mul(float4(position, 1.0f), shadow_data.uv_from_ndc);
    
    shadow_pos.xyz /= shadow_pos.w;
    
    
    float dx = shadow_data.inverse_texture_size.x;
    float dy = shadow_data.inverse_texture_size.y;
    
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };
    
    Texture2D shadow_texture = g_texture_data[shadow_data.shadow_texture_index];
    
    float percent_lit = 0.0f;
    [unroll]
    for (int i = 0; i < 9; ++i)
        percent_lit += shadow_texture.SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, shadow_pos.xy + offsets[i], shadow_pos.z).r;
    
    if(is_static_light == true)
    {
        shadow_texture = g_texture_data[shadow_data.shadow_texture_index + 1];
        float static_percent_lit = 0.0f;
        
        [unroll]
        for (int i = 0; i < 9; ++i)
            static_percent_lit += shadow_texture.SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, shadow_pos.xy + offsets[i], shadow_pos.z).r;
        
        percent_lit = min(percent_lit, static_percent_lit);
    }
    
    return percent_lit / 9.0f;
}

float CalcShadowCubeFactor(float3 to_pixel, bool is_static_light, ShadowTextureData shadow_data)
{    
    float3 abs_to_pixel = abs(to_pixel);
    
    float z = max(abs_to_pixel.x, max(abs_to_pixel.y, abs_to_pixel.z));
    float depth = (shadow_data.inverse_texture_size.x * z + shadow_data.inverse_texture_size.y) / z;
    
    float shadow_factor = g_texture_cube_data[shadow_data.shadow_texture_index].SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, to_pixel, depth);
    
    if(is_static_light == true)
        shadow_factor = min(shadow_factor, g_texture_cube_data[shadow_data.shadow_texture_index + 1].SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, to_pixel, depth));
    
    return shadow_factor;
}

float CalcCascadeShadow(float3 position, ShadowTextureData shadow_data, CascadeShadowTextureData cascade_shadow_data)
{
    float4 shadow_pos = mul(float4(position, 1.0f), shadow_data.uv_from_ndc);
    
    if(shadow_pos.z >= 1.0f)
        return 1.0f;
    
    float4 cascade_x_pos = (cascade_shadow_data.cascade_offset_x + shadow_pos.xxxx) * cascade_shadow_data.cascade_scale;
    float4 cascade_y_pos = (cascade_shadow_data.cascade_offset_y + shadow_pos.yyyy) * cascade_shadow_data.cascade_scale;
    
    float4 in_cascade_x = abs(cascade_x_pos) <= 1.0f;
    float4 in_cascade_y = abs(cascade_y_pos) <= 1.0f;
    float4 in_cascade = in_cascade_x * in_cascade_y;
    
    float4 best_cascade_mask = in_cascade;
    best_cascade_mask.yzw = (1.0f - best_cascade_mask.x) * best_cascade_mask.yzw;
    best_cascade_mask.zw = (1.0f - best_cascade_mask.y) * best_cascade_mask.zw;
    best_cascade_mask.w = (1.0f - best_cascade_mask.z) * best_cascade_mask.w;
    float best_cascade = dot(best_cascade_mask, float4(0.0f, 1.0f, 2.0f, 3.0f));
    
    float3 uvd;
    uvd.x = dot(cascade_x_pos, best_cascade_mask);
    uvd.y = dot(cascade_y_pos, best_cascade_mask);
    uvd.z = shadow_pos.z;
    
    uvd.xy = 0.5f * uvd.xy + 0.5f;
    uvd.y = 1.0f - uvd.y;
    
    float shadow = g_texture_array_data[shadow_data.shadow_texture_index].SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, float3(uvd.xy, best_cascade), uvd.z);
    
    shadow = saturate(shadow + 1.0f - any(best_cascade_mask));
    
    return shadow;
}

#endif //__SHADOW_HLSL__