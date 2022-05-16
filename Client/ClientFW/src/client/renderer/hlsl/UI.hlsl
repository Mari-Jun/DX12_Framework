#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __UI_HLSL__
#define __UI_HLSL__

struct VS_RENDER_TEXTURE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

static const float2 s_rt_base_pos[4] = { float2(-1.0f, 1.0f), float2(1.0f, 1.0f), float2(-1.0f, -1.0f), float2(1.0f, -1.0f) };
static const float2 s_rt_base_uv[4] = { float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f) };

VS_RENDER_TEXTURE_OUTPUT VSRenderTexture(uint vertex_id : SV_VertexID) 
{
    VS_RENDER_TEXTURE_OUTPUT output;
    
    output.sv_position = float4(s_rt_base_pos[vertex_id], 0.0f, 1.0f);
    output.uv = s_rt_base_uv[vertex_id];
    
    return output;
}

float4 PSRenderMainUI(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    //float3 normal = g_texture_data[g_gbuffer_texture_indices[1]].Sample(g_sampler_point_wrap, input.uv).xyz;
    //normal = normalize(normal * 2.0f - 1.0f);
    //return float4(abs(normal), 1.0f);
    
    //юс╫ц
    return g_texture_data[g_final_texture_index].Sample(g_sampler_point_wrap, input.uv);
}

struct VS_UI_INPUT
{
    float2 position : POSITION;
    float2 size : SIZE;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
    float2 coordinate : TEXCOORD;
    float2 tilling : TILLING;
};

struct GS_UI_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
};

VS_UI_INPUT VSRenderUI(VS_UI_INPUT input)
{
    return input;
}

static float2 s_ui_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

[maxvertexcount(4)]
void GSRenderUI(point VS_UI_INPUT input[1], inout TriangleStream<GS_UI_OUTPUT> out_stream)
{
    float2 vertices[4];
    
    vertices[0] = float2(input[0].position.x, input[0].position.y - input[0].size.y);
    vertices[1] = float2(input[0].position.x, input[0].position.y);
    vertices[2] = float2(input[0].position.x + input[0].size.x, input[0].position.y - input[0].size.y);
    vertices[3] = float2(input[0].position.x + input[0].size.x, input[0].position.y);
    
    GS_UI_OUTPUT output;
    
    output.texture_index = input[0].texture_index;
    output.color = input[0].color;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(float4(vertices[i], 0.0f, 1.0f), g_projection);
        output.sv_position.z = 0.0f;
        output.uv = s_ui_uvs[i] * input[0].tilling + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

float4 PSRenderUI(GS_UI_OUTPUT input) : SV_TARGET
{
    if (input.texture_index >= 0)
        return g_texture_data[input.texture_index].Sample(g_sampler_point_wrap, input.uv) * input.color;
    else
        return input.color;
}

#endif // __UI_HLSL__