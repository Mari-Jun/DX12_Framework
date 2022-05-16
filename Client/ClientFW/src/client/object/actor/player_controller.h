#pragma once
#include "client/object/actor/controller.h"

namespace client_fw
{
	class PlayerCameraManager;
	class RenderCameraComponent;

	class PlayerController : public Controller
	{
	public:
		PlayerController(const std::string& name = "player controller");
		virtual ~PlayerController();

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		void AddPitchInput(float value);
		void AddYawInput(float value);
		void AddRollInput(float value);

	protected:
		virtual void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterAxisEvent(const std::string& name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

	private:
		SPtr<RenderCameraComponent> m_original_camera_component;
		SPtr<RenderCameraComponent> m_custom_camera_component;
		float m_pitch_speed_scale = 4.0f;
		float  m_yaw_speed_scale = 4.0f;
		float m_roll_speed_scale = 4.0f;

	public:
		const SPtr<RenderCameraComponent>& GetPlayerCamera() const;
		//사용자 정의 카메라를 설정한다.
		void SetPlayerCamera(const SPtr<RenderCameraComponent>& camera_comp);

		void SetPitchSpeedScale(float scale) { m_pitch_speed_scale = scale; }
		void SetYawSpeedScale(float scale) { m_yaw_speed_scale = scale; }
		void SetRollSpeedScale(float scale) { m_roll_speed_scale = scale; }

		virtual void Possess(const SPtr<Pawn>& pawn) override;
		virtual void UnPossess() override;

	private:
		void SetControlledCamera(const SPtr<RenderCameraComponent>& camera_comp);
	};
}


