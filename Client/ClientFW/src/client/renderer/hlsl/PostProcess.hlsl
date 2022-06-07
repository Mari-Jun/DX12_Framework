#include "Resource.hlsl"

Texture2D g_input : register(t0, space0);
RWTexture2D<float4> g_output :register(u0, space0);

[numthreads(32, 1, 1)]
void CS(uint3 dispatch_thread_id : SV_DispatchThreadID )
{
    //g_output[dispatch_thread_id.xy] = g_input[dispatch_thread_id.xy];
}