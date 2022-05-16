#pragma once
#include "client/object/component/light/core/local_light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
    constexpr static INT s_spot_light_max_texture_size = 4096;

    class SpotLightComponent : public LocalLightComponent
    {
    public:
        SpotLightComponent(const std::string& name = "spot light component",
            const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kSpotLight));
        virtual ~SpotLightComponent() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

    private:
        virtual void UpdateWorldMatrix() override;
        virtual void UpdateLocalMatrix() override;
        virtual void UpdateOrientedBox() override;
        virtual void SetVisiblity(bool value) override;

    private:
        float m_cone_inner_angle = math::ToRadian(0.0f); 
        float m_cone_outer_angle = math::ToRadian(44.0f); 

    public:
        float GetConeInnerAngle() const { return m_cone_inner_angle; }
        void SetConeInnerAngle(float degrees);
        float GetConeOuterAngle() const { return m_cone_outer_angle; }
        void SetConeOuterAngle(float degrees);


    private:
        SPtr<ShadowCameraComponent> m_shadow_camera;

    private:
        virtual void UpdateShadowTextureSize() override;
        virtual void UpdateShadowCameraProjection() override;

    public:
        const SPtr<ShadowCameraComponent>& GetShadowCamera() const { return m_shadow_camera; }

    };
}
