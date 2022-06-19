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

[numthreads(16, 16, 1)]
void CSSobelEdge(int3 dispatch_thread_id : SV_DispatchThreadID)
{
    float4 neighbor[3][3];
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            int2 xy = dispatch_thread_id.xy + int2(-1 + y, -1 + x);
            neighbor[y][x] = g_input[xy];
        }
    }
    
    float4 gx = -1.0f * neighbor[0][0] - 2.0f * neighbor[1][0] - 1.0f * neighbor[2][0] +
    1.0f * neighbor[0][2] + 2.0f * neighbor[1][2] + 1.0f * neighbor[2][2];
    
    float4 gy = -1.0f * neighbor[2][0] - 2.0f * neighbor[2][1] - 1.0f * neighbor[2][2] +
    1.0f * neighbor[0][0] + 2.0f * neighbor[0][1] + 1.0f * neighbor[0][2];

    float4 mag = sqrt(gx * gx + gy * gy);
    mag = 1.0f - saturate(dot(mag.rgb, float3(0.299f, 0.587f, 0.114f)));
    
    g_output[dispatch_thread_id.xy] = g_input[dispatch_thread_id.xy] * mag;
}