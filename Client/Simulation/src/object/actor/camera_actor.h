#pragma once
#include <client/object/actor/core/actor.h>
#include <client/object/actor/pawn.h>
#include <client/object/actor/player_controller.h>

namespace client_fw
{
    class StaticMeshComponent;
    class PawnMovementComponent;
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

    class CameraPawnUILayer;

    class CameraPawn : public Pawn
    {
    public:
        CameraPawn(const std::string& name = "camera pawn");
        virtual ~CameraPawn() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

        virtual void AddMovementInput(const Vec3& direction, float scale) override;

        virtual void Possess(const SPtr<PlayerController>& controller) override;

    protected:
        SPtr<StaticMeshComponent> m_static_mesh_component;
        SPtr<RenderCameraComponent> m_render_camera_component;
        SPtr<PawnMovementComponent> m_movement_component;
        SPtr<CameraPawnUILayer> m_ui_layer;
        SPtr<WidgetComponent> m_widget_component;

    public:
        const SPtr<RenderCameraComponent>& GetRenderCameraComponent() const { return m_render_camera_component; }
    };

    class CameraPlayerController : public PlayerController
    {
    public:
        CameraPlayerController(bool use_event = true);
        virtual ~CameraPlayerController() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

    private:
        bool m_use_event;
    };
}



