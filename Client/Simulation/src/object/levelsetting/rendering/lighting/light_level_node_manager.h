#pragma once
#include "object/levelsetting/core/level_setting_manager.h"

namespace simulation
{
    class LightLevel;

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
}



