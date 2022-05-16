#include "resource.hlsl"
#include "light.hlsl"
#include "ui.hlsl"

#ifndef __DEFERRED_HLSL__
#define __DEFERRED_HLSL__

//임시
//아직 조명적용을 하지 않기 떄문에 그냥 Texture전달로 끝

struct GBufferData
{
    float4 base_color;
    float3 normal;
    float4 additional_info;
    float3 position;
    float depth;
};

struct GBufferDataWithoutBaseColor
{
    float3 normal;
    float4 additional_info;
    float3 position;
    float depth;
};

float ConvertToLinearDepth(float depth)
{
    return g_perspective_values.z / (depth + g_perspective_values.w);
}

float3 GetWorldPositionFromCSPos(float2 cs_pos, float depth)
{
    float3 position;
    
    position.xy = cs_pos.xy * g_perspective_values.xy * depth;
    position.z = depth;
    
    return mul(float4(position, 1.0f), g_inverse_view).xyz;
}

float3 GetWorldPositionFromUV(float2 uv, float depth)
{
    uv.x = uv.x * 2.0f - 1.0f;
    uv.y = (1.0f - uv.y) * 2.0f - 1.0f;
   
    return GetWorldPositionFromCSPos(uv, depth);
}

GBufferData UnpackGBuffer(float2 uv)
{
    GBufferData data;
    
    data.base_color = g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, uv);
    data.normal = g_texture_data[g_gbuffer_texture_indices[1]].Sample(g_sampler_point_wrap, uv).xyz;
    data.normal = normalize(data.normal * 2.0f - 1.0f);
    data.additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Sample(g_sampler_point_wrap, uv);
    data.depth = g_texture_data[g_gbuffer_texture_indices[3]].Sample(g_sampler_point_wrap, uv).x;
    data.depth = ConvertToLinearDepth(data.depth);
    data.position = GetWorldPositionFromUV(uv, data.depth);
    
    return data;
}

GBufferDataWithoutBaseColor UnpackGBufferWithoutBaseColorFromUV(float2 uv)
{
    GBufferDataWithoutBaseColor data;
    
    data.normal = g_texture_data[g_gbuffer_texture_indices[1]].Sample(g_sampler_point_wrap, uv).xyz;
    data.normal = normalize(data.normal * 2.0f - 1.0f);
    data.additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Sample(g_sampler_point_wrap, uv);
    data.depth = g_texture_data[g_gbuffer_texture_indices[3]].Sample(g_sampler_point_wrap, uv).x;
    data.depth = ConvertToLinearDepth(data.depth);
    data.position = GetWorldPositionFromUV(uv, data.depth);
    
    return data;
}

GBufferDataWithoutBaseColor UnpackGBufferWithoutBaseColorFromLocation(int3 location, float2 cs_pos)
{
    GBufferDataWithoutBaseColor data;
    
    data.normal = g_texture_data[g_gbuffer_texture_indices[1]].Load(location).xyz;
    data.normal = normalize(data.normal * 2.0f - 1.0f);
    data.additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Load(location);
    data.depth = g_texture_data[g_gbuffer_texture_indices[3]].Load(location).x;
    data.depth = ConvertToLinearDepth(data.depth);
    data.position = GetWorldPositionFromCSPos(cs_pos, data.depth);
    
    return data;
}

Material GetMaterial(float4 base_color, GBufferDataWithoutBaseColor g_buffer_data)
{
    Material material;
    
    material.base_color = base_color.xyz;
    material.normal = g_buffer_data.normal;
    material.roughness = g_buffer_data.additional_info.x;
    material.metalic = g_buffer_data.additional_info.y;
    
    return material;
}

float4 PSRenderTexture(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    return float4(g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv).xyz, 1.0f);
}

float4 PSRenderTextureWithDirectionalLight(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    float4 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv);
    
    if(base_color.a > 0.0f)
    {
        GBufferDataWithoutBaseColor g_buffer_data = UnpackGBufferWithoutBaseColorFromUV(input.uv);
             
        Material material = GetMaterial(base_color, g_buffer_data);
        
        DirectionalLightShadowInfo shadow_info;
        shadow_info.render_camera_index = g_render_camera_index;
    
        float3 color = float3(0.f, 0.f, 0.f);
        [unroll(4)]
        for (uint i = 0; i < g_num_of_directional_light; ++i)
        {
            LightData light_data = g_light_data[i];
            
            DirectionalLight light;
            light.light_color = light_data.light_color;
            light.direction = light_data.light_direction;
            light.use_shadow = light_data.use_shadow > 0;
            shadow_info.shadow_texture_data_index = light_data.shadow_texture_data_index;
            
            color += CalcDiretionalLight(g_buffer_data.position, material, light, shadow_info);
        }
    
     
        float3 ambient = 0.03f * material.base_color;
        color += ambient;
        color = color / (color + 1.0f);
        color = pow(abs(color), (1.0f / 2.2f));
    
        return float4(color, 1.0f);
    }
    else
    {
        return float4(base_color.xyz, 1.0f);
    }  
}

struct VS_LOCAL_LIGHT_OUTPUT
{
    float4 sv_position : SV_POSITION;
    uint instance_id : SV_InstanceID;
};

struct HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HS_LOCAL_LIGHT_OUTPUT
{
    float3 position : POSITION;
    uint instance_id : SV_InstanceID;
};

struct DS_LOCAL_LIGHT_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 cs_position : CS_POSITION;
    uint light_index : LIGHT_INDEX;
};

VS_LOCAL_LIGHT_OUTPUT VSLocalLight(uint instance_id : SV_InstanceID)
{
    VS_LOCAL_LIGHT_OUTPUT output;
    
    output.sv_position = float4(0.0f, 0.0f, 0.0f, 1.0f);
    output.instance_id = instance_id;
    
    return output;
}

//
// Draw Point Light
//

HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT ConstantHSPointLight()
{
    HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT output;
    
    float tess_factor = 18.0f;
    output.edges[0] = output.edges[1] =
    output.edges[2] = output.edges[3] = tess_factor;
    output.inside[0] = output.inside[1] = tess_factor;
    
    return output;
}

static float3 hemil_dir[2] = { float3(1.0f, 1.0f, 1.0f), float3(-1.0f, 1.0f, -1.0f) };

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHSPointLight")]
HS_LOCAL_LIGHT_OUTPUT HSPointLight(InputPatch<VS_LOCAL_LIGHT_OUTPUT, 1> input, uint patch_id : SV_PrimitiveID)
{
    HS_LOCAL_LIGHT_OUTPUT output;
    
    output.position = hemil_dir[patch_id];
    output.instance_id = input[0].instance_id;
    
    return output;
}

[domain("quad")]
DS_LOCAL_LIGHT_OUTPUT DSPointLight(HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT input, float2 uv : SV_DomainLocation, OutputPatch <HS_LOCAL_LIGHT_OUTPUT, 4> quad)
{
    float2 clip_space_uv = uv.xy * 2.0f - 1.0f;
    
    float2 clip_space_abs_uv = abs(clip_space_uv.xy);
    float max_length = max(clip_space_abs_uv.x, clip_space_abs_uv.y);
    
    float3 position = normalize(float3(clip_space_uv, (max_length - 1.0f)) * quad[0].position);
    
    DS_LOCAL_LIGHT_OUTPUT output;
    
    InstanceData instance_data = g_instance_data[quad[0].instance_id];
    
    output.sv_position = mul(mul(float4(position, 1.0f), instance_data.world), g_view_projection);
    output.cs_position = output.sv_position.xyw;
    output.light_index = instance_data.additional_info;
    
    return output;
}

float4 PSPointLight(DS_LOCAL_LIGHT_OUTPUT input) : SV_TARGET
{   
    int3 location = int3(input.sv_position.xy, 0);
    float4 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Load(location);
    
    if (base_color.a > 0.0f)
    {
        GBufferDataWithoutBaseColor g_buffer_data = UnpackGBufferWithoutBaseColorFromLocation(location, input.cs_position.xy / input.cs_position.z);
        
        Material material = GetMaterial(base_color, g_buffer_data);
    
        float3 color = float3(0.f, 0.f, 0.f);
        
        LightData light_data = g_light_data[input.light_index];
        PointLight light;
            
        light.light_color = light_data.light_color;
        light.position = light_data.light_position;
        light.attenuation_radius = light_data.attenuation_radius;
        light.use_shadow = light_data.use_shadow > 0;
        light.is_static_light = light_data.is_static_light > 0;
        
        color += CalcPointLight(g_buffer_data.position, material, light, light_data.shadow_texture_data_index);
        
        color = color / (color + 1.0f);
        color = pow(abs(color), (1.0f / 2.2f));
        
        return float4(color, 1.0f);
    
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

//
// Draw Spot Light
//

HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT ConstantHSSpotLight()
{
    HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT output;
    
    float tess_factor = 18.0f;
    output.edges[0] = output.edges[1] =
    output.edges[2] = output.edges[3] = tess_factor;
    output.inside[0] = output.inside[1] = tess_factor;
    
    return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHSSpotLight")]
HS_LOCAL_LIGHT_OUTPUT HSSpotLight(InputPatch<VS_LOCAL_LIGHT_OUTPUT, 1> input)
{
    HS_LOCAL_LIGHT_OUTPUT output;
    
    output.position = float3(0.0f, 0.0f, 0.0f);
    output.instance_id = input[0].instance_id;
    
    return output;
}

static const float cylinder_portion = 0.2f;
static const float expend_amount = (1.0f + cylinder_portion);


[domain("quad")]
DS_LOCAL_LIGHT_OUTPUT DSSpotLight(HS_LOCAL_LIGHT_CONSTANT_DATA_OUTPUT input, float2 uv : SV_DomainLocation, OutputPatch<HS_LOCAL_LIGHT_OUTPUT, 4> quad)
{
    float2 clip_space_uv = uv.xy * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
    
    float2 clip_space_abs_uv = abs(clip_space_uv.xy);
    float max_length_uv = max(clip_space_abs_uv.x, clip_space_abs_uv.y);
    
    float2 clip_space_abs_pos = saturate(clip_space_abs_uv * expend_amount);
    float max_length_pos = max(clip_space_abs_pos.x, clip_space_abs_pos.y);
    float2 clip_space_pos = sign(clip_space_uv.xy) * clip_space_abs_pos;
    
    InstanceData instance_data = g_instance_data[quad[0].instance_id];
    uint light_index = instance_data.additional_info;
    
    float angle = g_light_data[light_index].cone_outer_angle;
    
    float3 half_sphere_pos = normalize(float3(clip_space_pos.xy, 1.0f - max_length_pos));
    half_sphere_pos = normalize(float3(half_sphere_pos.xy * sin(angle), cos(angle)));
    
    float cylinder_offset_z = saturate((max_length_uv * expend_amount - 1.0f) / cylinder_portion);
    
    float3 position = float3(half_sphere_pos.xy * (1.0f - cylinder_offset_z), half_sphere_pos.z - cylinder_offset_z * cos(angle));
    
    DS_LOCAL_LIGHT_OUTPUT output;
    
    output.sv_position = mul(mul(float4(position, 1.0f), instance_data.world), g_view_projection);
    output.cs_position = output.sv_position.xyw;
    output.light_index = light_index;

    
    return output;
}

float4 PSSpotLight(DS_LOCAL_LIGHT_OUTPUT input) : SV_TARGET
{
    int3 location = int3(input.sv_position.xy, 0);
    float4 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Load(location);
    
    if (base_color.a > 0.0f)
    {        
        GBufferDataWithoutBaseColor g_buffer_data = UnpackGBufferWithoutBaseColorFromLocation(location, input.cs_position.xy/ input.cs_position.z);
        
        Material material = GetMaterial(base_color, g_buffer_data);
        
        float3 color = float3(0.f, 0.f, 0.f);
        
        LightData light_data = g_light_data[input.light_index];
        SpotLight light;
            
        light.light_color = light_data.light_color;
        light.position = light_data.light_position;
        light.direction = light_data.light_direction;
        light.attenuation_radius = light_data.attenuation_radius;
        light.inner_angle = light_data.cone_inner_angle;
        light.outer_angle = light_data.cone_outer_angle;
        light.use_shadow = light_data.use_shadow > 0;
        light.is_static_light = light_data.is_static_light > 0;
                        
        color += CalcSpotLight(g_buffer_data.position, material, light, light_data.shadow_texture_data_index);
        
        color = color / (color + 1.0f);
        color = pow(abs(color), (1.0f / 2.2f));
        
        return float4(color, 1.0f);
    
    }
    else
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}

#endif // __DEFERRED_HLSL__