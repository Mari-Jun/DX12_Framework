#include "Resource.hlsl"

Texture2D g_input : register(t0, space0);
RWTexture2D<float4> g_output :register(u0, space0);

static const int g_max_blur_radius = 10;

cbuffer cbPostProcessingData : register(b0, space0)
{
    int g_blur_radius;
    float3 g_pp_data_padding_0;
    float4 g_blur_weights[(g_max_blur_radius * 2 + 1) / 4];
    float3 g_pp_data_padding_1;
};

// Test post processing

[numthreads(32, 1, 1)]
void CS(int3 dispatch_thread_id : SV_DispatchThreadID )
{
    g_output[dispatch_thread_id.xy] = g_input[dispatch_thread_id.xy];
}


// Blur
groupshared float4 g_texture_cache[256 + 2 * g_max_blur_radius];

[numthreads(256, 1, 1)]
void CSHorizontalBlur(int3 group_thread_id : SV_GroupThreadID, int3 dispatch_thread_id : SV_DispatchThreadID)
{
    if (group_thread_id.x < g_blur_radius)
    {
        int x = max(dispatch_thread_id.x - g_blur_radius, 0);
        g_texture_cache[group_thread_id.x] = g_input[int2(x, dispatch_thread_id.y)];
    }
    
    if (group_thread_id.x >= 256 - g_blur_radius)
    {
        int x = min(dispatch_thread_id.x + g_blur_radius, g_input.Length.x - 1);
        g_texture_cache[group_thread_id.x + 2 * g_blur_radius] = g_input[int2(x, dispatch_thread_id.y)];
    }
    
    g_texture_cache[group_thread_id.x + g_blur_radius] = g_input[min(dispatch_thread_id.xy, g_input.Length.xy - 1)];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blur_color = float4(0, 0, 0, 1);
    
    for (int i = -g_blur_radius; i <= g_blur_radius; ++i)
    {
        blur_color += g_blur_weights[(i + g_blur_radius) / 4][(i + g_blur_radius) % 4] * g_texture_cache[group_thread_id.x + g_blur_radius + i];
    }
    
    g_output[dispatch_thread_id.xy] = blur_color;
}

[numthreads(1, 256, 1)]
void CSVerticalBlur(int3 group_thread_id : SV_GroupThreadID, int3 dispatch_thread_id : SV_DispatchThreadID)
{
    if (group_thread_id.y < g_blur_radius)
    {
        int y = max(dispatch_thread_id.y - g_blur_radius, 0);
        g_texture_cache[group_thread_id.y] = g_input[int2(dispatch_thread_id.x, y)];
    }
    
    if (group_thread_id.y >= 256 - g_blur_radius)
    {
        int y = min(dispatch_thread_id.y + g_blur_radius, g_input.Length.y - 1);
        g_texture_cache[group_thread_id.y + 2 * g_blur_radius] = g_input[int2(dispatch_thread_id.x, y)];
    }
    
    g_texture_cache[group_thread_id.y + g_blur_radius] = g_input[min(dispatch_thread_id.xy, g_input.Length.xy - 1)];
    
    GroupMemoryBarrierWithGroupSync();
    
    float4 blur_color = float4(0, 0, 0, 1);
    
    for (int i = -g_blur_radius; i <= g_blur_radius; ++i)
    {
        blur_color += g_blur_weights[(i + g_blur_radius) / 4][(i + g_blur_radius) % 4] * g_texture_cache[group_thread_id.y + g_blur_radius + i];
    }
    
    g_output[dispatch_thread_id.xy] = blur_color;
}