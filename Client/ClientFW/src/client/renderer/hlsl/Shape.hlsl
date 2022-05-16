#include "Opaque.hlsl"

struct VS_SHAPE_IN
{
    float3 position : POSITION;
    float3 size : SIZE;
    float3 forward : FORWARD;
    float3 right : RIGHT;
    float3 up : UP;
};

struct VS_SHAPE_OUTPUT
{
    float4 sv_position : SV_POSITION;
};

VS_SHAPE_OUTPUT VSShape(VS_SHAPE_IN input)
{
    VS_SHAPE_OUTPUT output;
    output.sv_position = mul(mul(float4(input.position, 1.f), g_view), g_projection);
    return output;
}

//[maxvertexcount()]

float4 PSShape(VS_SHAPE_OUTPUT input) : SV_TARGET
{
    return float4(1.f, 1.f, 0.f, 1.f);
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}