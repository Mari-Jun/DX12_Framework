#pragma once
#include "client/object/actor/core/actor.h"
#include "client/object/actor/controller.h"

namespace client_fw
{
	class Controller;

	class Pawn : public Actor
	{
	public:
		Pawn(const std::string& name = "pawn");
		virtual ~Pawn() = default;

		virtual void Update(float delta_time) override;

		virtual void AddMovementInput(const Vec3& direction, float scale) {}

	protected:
		virtual void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterAxisEvent(const std::string& name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

		bool AddControllerPitchInput(float value);
		bool AddControllerYawInput(float value);
		bool AddControllerRollInput(float value);

	protected:
		WPtr<Controller> m_controller;

	private:
		bool m_is_need_update = false;
		bool m_use_controller_pitch = false;
		bool m_use_controller_yaw = false;
		bool m_use_controller_roll = false;

	public:
		void SetController(const SPtr<Controller>& controller);
		bool IsNeedUpdate() const { return m_is_need_update; }
		bool IsUseControllerPitch() const { return m_use_controller_pitch; }
		bool IsUseControllerYaw() const { return m_use_controller_yaw; }
		bool IsUseControllerRoll() const { return m_use_controller_roll; }
		void SetUseControllerPitch(bool value) { m_use_controller_pitch = value; }
		void SetUseControllerYaw(bool value) { m_use_controller_yaw = value; }
		void SetUseControllerRoll(bool value) { m_use_controller_roll = value; }
	};
}



