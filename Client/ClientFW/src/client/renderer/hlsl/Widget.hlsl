#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __WIDGET_HLSL__
#define __WIDGET_HLSL__

struct VS_WORLD_WIDGET_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
    float2 coordinate : TEXCOORD;
    float2 tilling : TILLING;
    float3 right : RIGHT;
    float3 up : UP;
};

struct VS_PIVOT_WIDGET_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
    float2 coordinate : TEXCOORD;
    float2 tilling : TILLING;
    float2 pivot : PIVOT;
};

struct GS_WIDGET_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
};

VS_WORLD_WIDGET_INPUT VSWorldWidget(VS_WORLD_WIDGET_INPUT input)
{
    return input;
}

VS_PIVOT_WIDGET_INPUT VSPivotWidget(VS_PIVOT_WIDGET_INPUT input)
{
    return input;
}

static float2 s_widget_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

void GetWidgetBillboardConrers(float3 position, float2 size, float2 pivot, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(look, g_view._12_22_32));
    up = normalize(cross(right, look));
    
    vertices[0] = float4(position - size.y * up, 1.0f);
    vertices[1] = float4(position, 1.0f);
    vertices[2] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position + size.x * right, 1.0f);
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        vertices[i].xyz -= pivot.x * right;
        vertices[i].xyz += pivot.y * up;
    }
}

void GetWidgetFixUpBillboardConrers(float3 position, float2 size, float2 pivot, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    up = float3(0.0f, 1.0f, 0.0f);
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(look, up));
    
    vertices[0] = float4(position - size.y * up, 1.0f);
    vertices[1] = float4(position, 1.0f);
    vertices[2] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position + size.x * right, 1.0f);
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        vertices[i].xyz -= pivot.x * right;
        vertices[i].xyz += pivot.y * up;
    }
}

[maxvertexcount(4)]
void GSWorldWidget(point VS_WORLD_WIDGET_INPUT input[1], inout TriangleStream<GS_WIDGET_OUTPUT> out_stream)
{
    float3 vertices[4];
    
    float3 pos = input[0].position;
    float3 right = input[0].right * input[0].size.x;
    float3 up = input[0].up * input[0].size.y;
    vertices[0] = pos - up;
    vertices[1] = pos;
    vertices[2] = pos - up + right;
    vertices[3] = pos + right;
    
    GS_WIDGET_OUTPUT output;
    
    output.normal = normalize(cross(input[0].right, input[0].up));
    output.texture_index = input[0].texture_index;
    output.color = input[0].color;

    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(float4(vertices[i], 1.0f), g_view_projection);
        output.uv = s_widget_uvs[i] * input[0].tilling + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSBillboardWidget(point VS_PIVOT_WIDGET_INPUT input[1], inout TriangleStream<GS_WIDGET_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetWidgetBillboardConrers(input[0].position, input[0].size, input[0].pivot, vertices, look, right, up);
    
    GS_WIDGET_OUTPUT output;
    
    output.normal = look;
    output.texture_index = input[0].texture_index;
    output.color = input[0].color;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_widget_uvs[i] * input[0].tilling + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

[maxvertexcount(4)]
void GSFixUpBillboardWidget(point VS_PIVOT_WIDGET_INPUT input[1], inout TriangleStream<GS_WIDGET_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetWidgetFixUpBillboardConrers(input[0].position, input[0].size, input[0].pivot, vertices, look, right, up);
    
    GS_WIDGET_OUTPUT output;
    
    output.normal = look;
    output.texture_index = input[0].texture_index;
    output.color = input[0].color;
    
    [unroll]
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(vertices[i], g_view_projection);
        output.uv = s_widget_uvs[i] * input[0].tilling + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSOpaqueWidget(GS_WIDGET_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    if (input.texture_index >= 0)
        output.base_color = g_texture_data[input.texture_index].Sample(g_sampler_point_wrap, input.uv) * input.color;
    else
        output.base_color = input.color;
    output.base_color.a = 0.0f;
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return output;
}

PS_GBUFFER_OUTPUT PSMaskedWidget(GS_WIDGET_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    float4 base_color;
    
    if (input.texture_index >= 0)
        base_color = g_texture_data[input.texture_index].Sample(g_sampler_point_wrap, input.uv) * input.color;
    else
        base_color = input.color;
    
    clip(base_color.a - MASKED_ALPHA);
    output.base_color = base_color;
    output.base_color.a = 0.0f;
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return output;
}

#endif // __WIDGET_HLSL__