#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/player_controller.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/component/render/widget_component.h>
#include <client/object/component/util/simple_movement_component.h>

#include "object/actor/camera_actor.h"
#include "object/ui/camear_actor_ui_layer.h"

namespace simulation
{
	CameraActor::CameraActor(eMobilityState mobility_state, const std::string& name)
		: Actor(mobility_state, name)
	{
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
		m_render_camera_component = CreateSPtr<RenderCameraComponent>();
		m_ui_layer = CreateSPtr<CameraActorUILayer>();
		m_widget_component = CreateSPtr<WidgetComponent>(m_ui_layer);
	}

	bool CameraActor::Initialize()
	{
		bool ret = true;

		m_static_mesh_component->SetLocalScale(0.2f);
		ret &= m_static_mesh_component->SetMesh("Contents/Meshes/SecurityCamera/SecurityCamera.obj");
		ret &= AttachComponent(m_static_mesh_component);

		m_render_camera_component->SetActive();
		m_render_camera_component->SetViewSize(IVec2(800, 600));
		ret &= AttachComponent(m_render_camera_component);

		m_ui_layer->SetCameraActor(std::static_pointer_cast<CameraActor>(shared_from_this()));
		m_widget_component->SetLocalPosition(Vec3(0.0f, 30.0f, 0.0f));
		m_widget_component->SetSize(Vec2(200.0f, 25.0f));
		ret &= AttachComponent(m_widget_component);

		return ret;
	}

	void CameraActor::Shutdown()
	{
	}

	void CameraActor::Update(float delta_time)
	{
	}

	CameraPawn::CameraPawn(const std::string& name)
		: Pawn(name)
	{
		m_static_mesh_component = CreateSPtr<StaticMeshComponent>();
		m_render_camera_component = CreateSPtr<RenderCameraComponent>();
		m_movement_component = CreateSPtr<SimpleMovementComponent>();
		m_ui_layer = CreateSPtr<CameraPawnUILayer>();
		m_widget_component = CreateSPtr<WidgetComponent>(m_ui_layer);
	}

	bool CameraPawn::Initialize()
	{
		bool ret = true;

		m_static_mesh_component->SetLocalScale(0.2f);
		ret &= m_static_mesh_component->SetMesh("Contents/Meshes/SecurityCamera/SecurityCamera.obj");
		ret &= AttachComponent(m_static_mesh_component);

		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(m_render_camera_component);
		ret &= AttachComponent(m_render_camera_component);

		SetUseControllerPitch(true);
		SetUseControllerYaw(true);
		SetUseControllerRoll(true);
		ret &= AttachComponent(m_movement_component);

		m_ui_layer->SetCameraPawn(std::static_pointer_cast<CameraPawn>(shared_from_this()));
		m_widget_component->SetLocalPosition(Vec3(0.0f, 30.0f, 0.0f));
		m_widget_component->SetSize(Vec2(200.0f, 25.0f));
		ret &= AttachComponent(m_widget_component);

		return ret;
	}

	void CameraPawn::Shutdown()
	{
	}

	void CameraPawn::Update(float delta_time)
	{
	}

	void CameraPawn::AddMovementInput(const Vec3& direction, float scale)
	{
		m_movement_component->AddInputVector(direction * scale);
	}

	void CameraPawn::Possess(const SPtr<PlayerController>& controller)
	{
		Pawn::Possess(controller);
		controller->SetPlayerCamera(m_render_camera_component);
	}

	CameraPlayerController::CameraPlayerController(bool use_event)
		: PlayerController("camera player controller")
		, m_use_event(use_event)
	{
	}

	bool CameraPlayerController::Initialize()
	{
		bool ret = PlayerController::Initialize();

		if (m_use_event)
		{
			RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
				[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetForward(), axis); return true; });
			RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
				[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetRight(), axis); return true; });
			RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
				[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetUp(), axis); return true; });
			RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
				[this](float axis)->bool {
					IVec2 relative_pos = Input::GetRelativeMousePosition();
					AddYawInput(axis * relative_pos.x);
					return true;
				});
			RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
				[this](float axis)->bool {
					IVec2 relative_pos = Input::GetRelativeMousePosition();
					AddPitchInput(axis * relative_pos.y);
					return true;
				});
		}

		return ret;
	}

	void CameraPlayerController::Shutdown()
	{
	}
}
