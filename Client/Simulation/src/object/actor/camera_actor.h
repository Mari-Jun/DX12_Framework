#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
    class StaticMeshComponent;
    class RenderCameraComponent;
    class WidgetComponent;
}

namespace simulation
{
    using namespace client_fw;

    class CameraActorUILayer;

    class CameraActor : public Actor
    {
    public:
        CameraActor(eMobilityState mobility_state = eMobilityState::kMovable, const std::string& name = "camera actor");
        virtual ~CameraActor() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    protected:
        SPtr<StaticMeshComponent> m_static_mesh_component;
        SPtr<RenderCameraComponent> m_render_camera_component;
        SPtr<CameraActorUILayer> m_ui_layer;
        SPtr<WidgetComponent> m_widget_component;

    public:
        const SPtr<RenderCameraComponent>& GetRenderCameraComponent() const { return m_render_camera_component; }
    };
}



