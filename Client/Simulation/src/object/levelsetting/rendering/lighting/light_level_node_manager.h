#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class LightLevel;
    class MultiplePointLightLevel;

    class LightLevelInitNodeManager : public LevelNodeManager<LightLevel>
    {
    public:
        LightLevelInitNodeManager();
        virtual ~LightLevelInitNodeManager() = default;

    private:
        INT m_direction_light_shadow_size = 4096;
        INT m_point_light_shadow_size = 1024;
        INT m_spot_light_shadow_size = 2048;

    public:
        INT GetDirectionalLightShadowSize() const { return m_direction_light_shadow_size; }
        INT GetPointLightShadowSize() const { return m_point_light_shadow_size; }
        INT GetSpotLightShadowSize() const { return m_spot_light_shadow_size; }
    };

    class LightLevelRuntimeNodeManager : public LevelNodeManager<LightLevel>
    {
    public:
        LightLevelRuntimeNodeManager();
        virtual ~LightLevelRuntimeNodeManager() = default;
    };

    class MultiplePointLightLevelInitNodeManager : public LevelNodeManager<MultiplePointLightLevel>
    {
    public:
        MultiplePointLightLevelInitNodeManager();
        virtual ~MultiplePointLightLevelInitNodeManager() = default;

    private:
        INT m_num_of_point_lights = 50;
        float m_offset = 500.f;
        bool m_use_shadow = false;
        bool m_update_intensity = false;

    public:
        INT GetNumOfPointLights() const { return m_num_of_point_lights; }
        float GetOffset() const { return m_offset; }
        bool IsUseShadow() const { return m_use_shadow; }
        bool IsUpdateIntensity() const { return m_update_intensity; }
    };

    class MultiplePointLightLevelRuntimeNodeManager : public LevelNodeManager<MultiplePointLightLevel>
    {
    public:
        MultiplePointLightLevelRuntimeNodeManager();
        virtual ~MultiplePointLightLevelRuntimeNodeManager() = default;
    };

    class MultipleSpotLightLevelInitNodeManager : public LevelNodeManager<MultipleSpotLightLevel>
    {
    public:
        MultipleSpotLightLevelInitNodeManager();
        virtual ~MultipleSpotLightLevelInitNodeManager() = default;

    private:
        INT m_num_of_spot_lights = 50;
        INT m_num_of_track = 1;
        float m_offset = 500.f;
        bool m_use_shadow = false;
        bool m_update_intensity = false;

    public:
        INT GetNumOfSpotLights() const { return m_num_of_spot_lights; }
        INT GetNumOfTrack() const { return m_num_of_track; }
        float GetOffset() const { return m_offset; }
        bool IsUseShadow() const { return m_use_shadow; }
        bool IsUpdateIntensity() const { return m_update_intensity; }
    };

    class MultipleSpotLightLevelRuntimeNodeManager : public LevelNodeManager<MultipleSpotLightLevel>
    {
    public:
        MultipleSpotLightLevelRuntimeNodeManager();
        virtual ~MultipleSpotLightLevelRuntimeNodeManager() = default;
    };
}



