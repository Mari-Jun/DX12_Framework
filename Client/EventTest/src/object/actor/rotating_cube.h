#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
	class RotatingMovementComponent;
	class WidgetComponent;
}

namespace event_test
{
	using namespace client_fw;
	
	class EnemyInfoUILayer;

	class RotatingCube final : public StaticMeshActor
	{
	public:
		RotatingCube();
		virtual ~RotatingCube() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
		virtual void ExecuteMessage(const SPtr<MessageEventInfo>& message) override;
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) override;

	private:
		SPtr<RotatingMovementComponent> m_rotating_component;
		SPtr<WidgetComponent> m_widget_component;
		SPtr<EnemyInfoUILayer> m_ui_layer;
		//Vec3 m_network_pos{0.0f,0.0f,0.0f};
		//Vec3 m_network_vec{ 0.0f,0.0f,0.0f };
		//float m_nw_speed = 225.f;
		
		std::function<void(float)> m_speed_change_function;
		float m_rotating_y_speed = 0.0f;

	public:
		void OnSpeedChangeFunction(std::function<void(float)> function) { m_speed_change_function = function; }
	};
}



