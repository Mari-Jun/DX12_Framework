#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
    constexpr static float s_max_attenuation_radius = 4096;

    class LocalLightComponent : public LightComponent
    {
    protected:
        LocalLightComponent(eLightType type, const std::string& name,
            const std::string& draw_shader_name);
        virtual ~LocalLightComponent() = default;

        virtual bool Initialize() override;

        virtual void RegisterToVisualOctree() override;
        virtual void UnregisterFromVisualOctree() override;

    protected:
        float m_attenuation_radius = 1000.0f;

    public:
        float GetAttenuationRadius() const { return m_attenuation_radius; }
        void SetAttenuationRadius(float radius);

    private:
        virtual void UpdateShadowCameraProjection() {}
    };

}

