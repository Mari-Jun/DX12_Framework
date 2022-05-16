#include "resource.hlsl"
#include "shadow.hlsl"

#ifndef __LIGHT_HLSL__
#define __LIGHT_HLSL__

struct Material
{
    float3 base_color;
    float3 normal;
    float metalic;
    float roughness;
};

struct DirectionalLight
{
    float3 light_color;
    float3 direction;
    bool use_shadow;
};

struct DirectionalLightShadowInfo
{
    uint shadow_texture_data_index;
    uint num_of_directional_light;
    uint render_camera_index;
    uint num_of_render_camera;
};

struct PointLight
{
    float3 light_color;
    float3 position;
    float attenuation_radius;
    bool use_shadow;
    bool is_static_light;
};

struct SpotLight
{
    float3 light_color;
    float3 direction;
    float3 position;
    float attenuation_radius;
    float inner_angle;
    float outer_angle;
    bool use_shadow;
    bool is_static_light;
};


float3 FresnelSchlick(float cos_theta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(clamp(1.0f - cos_theta, 0.0f, 1.0f), 5.0f);
}

float DistributionGGX(float3 normal, float3 half_way, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    
    float ndoth = max(dot(normal, half_way), 0.0f);
    float ndoth2 = ndoth * ndoth;
    
    float denom = (ndoth2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return a2 / denom;
}

float GeometrySchlickGGX(float ndotv, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) * 0.125f;
 
    float denom = ndotv * (1.0f - k) + k;
    
    return ndotv / denom;
}

float GeometrySmith(float3 normal, float3 to_camera, float3 to_light, float roughness)
{
    float ndotv = max(dot(normal, to_camera), 0.0f);
    float ndotl = max(dot(normal, to_light), 0.0f);
    float ggx2 = GeometrySchlickGGX(ndotv, roughness);
    float ggx1 = GeometrySchlickGGX(ndotl, roughness);
    
    return ggx1 * ggx2;
}

float3 CalcDiretionalLight(float3 position, Material material, DirectionalLight light, DirectionalLightShadowInfo shadow_info)
{
    float3 to_camera = normalize(g_camera_pos - position);
    
    float3 f0 = 0.04f;
    f0 = lerp(f0, material.base_color, material.metalic);
    
    float3 to_light = -light.direction;
    float3 half_way = normalize(to_camera + to_light);
    
    float3 radiance = light.light_color;
    
    float ndf = DistributionGGX(material.normal, half_way, material.roughness);
    float g = GeometrySmith(material.normal, to_camera, to_light, material.roughness);
    float3 f = FresnelSchlick(max(dot(half_way, to_camera), 0.0f), f0);
    
    float3 k_specular = f;
    float3 k_diffuse = 1.0f - k_specular;
    k_diffuse *= 1.0f - material.metalic;
    
    float ndotl = max(dot(material.normal, to_light), 0.0f);
    float ndotv = max(dot(material.normal, to_camera), 0.0f);
    
    float3 numerator = ndf * g * f;
    float denominator = 4.0f * max(ndotv, 0.0f) * max(ndotl, 0.0f);
    float3 specular = numerator / max(denominator, 0.001f);
    
    float shadow_factor = 1.0f;
    if(light.use_shadow == true)
    {
        uint shadow_texture_index = shadow_info.shadow_texture_data_index + shadow_info.render_camera_index;
        shadow_factor = CalcCascadeShadow(position, g_shadow_texture_data[shadow_texture_index], g_cascade_shadow_texture_data[shadow_texture_index]);
    }

    float3 lo = (k_diffuse * material.base_color / PI + specular) * radiance * ndotl * shadow_factor;
    
    return lo;
}

float3 CalcPointLight(float3 position, Material material, PointLight light, uint shadow_texture_data_index)
{
    float distance = length(light.position - position);
    if (distance > light.attenuation_radius)
        return float3(0.0f, 0.0f, 0.0f);

    float3 to_camera = normalize(g_camera_pos - position);
    
    float3 f0 = 0.04f;
    f0 = lerp(f0, material.base_color, material.metalic);
    
    float3 to_light = normalize(light.position - position);
    float3 half_way = normalize(to_camera + to_light);
    
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = light.light_color * attenuation;
      
    if (length(radiance) == 0.0f)
        return float3(0.0f, 0.0f, 0.0f);
    
    float ndf = DistributionGGX(material.normal, half_way, material.roughness);
    float g = GeometrySmith(material.normal, to_camera, to_light, material.roughness);
    float3 f = FresnelSchlick(max(dot(half_way, to_camera), 0.0f), f0);
    
    float3 k_specular = f;
    float3 k_diffuse = 1.0f - k_specular;
    k_diffuse *= 1.0f - material.metalic;
    
    float ndotl = max(dot(material.normal, to_light), 0.0f);
    float ndotv = max(dot(material.normal, to_camera), 0.0f);
    
    float3 numerator = ndf * g * f;
    float denominator = 4.0f * max(ndotv, 0.0f) * max(ndotl, 0.0f);
    float3 specular = numerator / max(denominator, 0.001f);
    
    float point_factor = 1.0f - saturate(distance / light.attenuation_radius);
    
    float3 lo = (k_diffuse * material.base_color / PI + specular) * radiance * ndotl * point_factor;
    
    float shadow_factor = 1.0f;
    if (light.use_shadow == true)
    {
        shadow_factor = CalcShadowCubeFactor(position - light.position, light.is_static_light, g_shadow_texture_data[shadow_texture_data_index]);
    }
    
    return lo * shadow_factor;
}

float3 CalcSpotLight(float3 position, Material material, SpotLight light, uint shadow_texture_data_index)
{
    float distance = length(light.position - position);
    if (distance > light.attenuation_radius)
        return float3(0.0f, 0.0f, 0.0f);
    
    float3 to_light = normalize(light.position - position);
    float cos_angle = dot(-to_light, light.direction);
    float cos_outer_angle = cos(light.outer_angle);
    
    if(cos_angle < cos_outer_angle)
        return float3(0.0f, 0.0f, 0.0f);
    
    float3 to_camera = normalize(g_camera_pos - position);
    
    float3 f0 = 0.04f;
    f0 = lerp(f0, material.base_color, material.metalic);
    
    float3 half_way = normalize(to_camera + to_light);
    
    float attenuation = 1.0f / (distance * distance);
    float3 radiance = light.light_color * attenuation;
    
    if (length(radiance) == 0.0f)
        return float3(0.0f, 0.0f, 0.0f);
    
    float ndf = DistributionGGX(material.normal, half_way, material.roughness);
    float g = GeometrySmith(material.normal, to_camera, to_light, material.roughness);
    float3 f = FresnelSchlick(max(dot(half_way, to_camera), 0.0f), f0);
    
    float3 k_specular = f;
    float3 k_diffuse = 1.0f - k_specular;
    k_diffuse *= 1.0f - material.metalic;
    
    float ndotl = max(dot(material.normal, to_light), 0.0f);
    float ndotv = max(dot(material.normal, to_camera), 0.0f);
    
    float3 numerator = ndf * g * f;
    float denominator = 4.0f * max(ndotv, 0.0f) * max(ndotl, 0.0f);
    float3 specular = numerator / max(denominator, 0.001f);
    
    float spot_factor = 1.0f;
    
    if (cos_angle < cos(light.inner_angle))
        spot_factor = saturate((cos_angle - cos(light.outer_angle)) / (cos(light.inner_angle) - cos(light.outer_angle)));
    
    spot_factor *= spot_factor;
        
    float3 lo = (k_diffuse * material.base_color / PI + specular) * radiance * ndotl * spot_factor;
    
    float shadow_factor = 1.0f;
    if (light.use_shadow == true)
    {
        shadow_factor = CalcShadowFactor(position, light.is_static_light, g_shadow_texture_data[shadow_texture_data_index]);
    }
    
    return lo * shadow_factor;
}

#endif //__LIGHT_HLSL__