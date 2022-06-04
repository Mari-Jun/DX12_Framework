#include "stdafx.h"
#include "client/object/actor/player_controller.h"
#include "client/input/input.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/actor/pawn.h"
#include "client/object/level/gamemode/game_mode_base.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"

namespace client_fw
{
	PlayerController::PlayerController(const std::string& name)
		: Controller(name)
	{
		m_original_camera_component = CreateSPtr<RenderCameraComponent>();
	}

	PlayerController::~PlayerController()
	{
	}

	bool PlayerController::Initialize()
	{
		const auto& cur_level = LevelManager::GetLevelManager().GetCurrentLevel();
		if (cur_level != nullptr)
		{
			if (cur_level->GetGameMode()->GetPlayerController() != shared_from_this())
			{
				LOG_ERROR("Could not create another player controller");
				return false;
			}
			return true;
		}
		return false;
	}

	void PlayerController::Shutdown()
	{
		if (m_controlled_pawn != nullptr)
			m_controlled_pawn->SetActorState(eActorState::kDead);
		m_custom_camera_component = nullptr;
		
	}

	void PlayerController::AddPitchInput(float value)
	{
		if (Input::GetInputMode() == eInputMode::kGameOnly)
			m_pitch_speed = math::ToRadian(value * m_pitch_speed_scale);
	}

	void PlayerController::AddYawInput(float value)
	{
		if(Input::GetInputMode() == eInputMode::kGameOnly)
			m_yaw_speed = math::ToRadian(value * m_yaw_speed_scale);
	}

	void PlayerController::AddRollInput(float value)
	{
		if (Input::GetInputMode() == eInputMode::kGameOnly)
			m_roll_speed = math::ToRadian(value * m_roll_speed_scale);
	}

	void PlayerController::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterPressedEvent(event_name, std::move(keys),
			func, consumption, eInputOwnerType::kPlayerController))
			RegisterInputEvent(event_name);
	}

	void PlayerController::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys, 
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterReleasedEvent(event_name, std::move(keys),
			func, consumption, eInputOwnerType::kPlayerController))
			RegisterInputEvent(event_name);
	}

	void PlayerController::RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterAxisEvent(event_name, std::move(keys),
			func, consumption, eInputOwnerType::kPlayerController))
			RegisterInputEvent(event_name);
	}

	const SPtr<RenderCameraComponent>& PlayerController::GetPlayerCamera() const
	{
		return (m_custom_camera_component == nullptr) ? m_original_camera_component : m_custom_camera_component;
	}

	void PlayerController::SetPlayerCamera(const SPtr<RenderCameraComponent>& camera_comp)
	{
		m_custom_camera_component = camera_comp;
		SetControlledCamera(m_custom_camera_component);
		m_controlled_pawn->DetachComponent(m_original_camera_component);
	}

	void PlayerController::Possess(const SPtr<Pawn>& pawn)
	{
		if (m_controlled_pawn != nullptr)
			UnPossess();
		Controller::Possess(pawn);
		if (m_custom_camera_component != nullptr)
		{
			SetControlledCamera(m_custom_camera_component);
		}
		else
		{
			m_controlled_pawn->AttachComponent(m_original_camera_component);
			SetControlledCamera(m_original_camera_component);
		}
	}

	void PlayerController::UnPossess()
	{
		if (m_custom_camera_component != nullptr)
		{
			m_custom_camera_component = nullptr;
			SetControlledCamera(m_original_camera_component);
		}
		else
		{
			m_controlled_pawn->DetachComponent(m_original_camera_component);
		}
		Controller::UnPossess();
	}

	void PlayerController::SetControlledCamera(const SPtr<RenderCameraComponent>& camera_comp)
	{
		const auto& cur_level = LevelManager::GetLevelManager().GetCurrentLevel();
		if (cur_level != nullptr &&
			cur_level->GetGameMode()->GetPlayerController() == shared_from_this())
		{
			camera_comp->SetOwnerController(shared_from_this());
			camera_comp->SetMainCamera();
		}
	}
}
