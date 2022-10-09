#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/rotating_movement_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/widget_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/event/messageevent/message_helper.h>
#include "rotating_cube.h"
#include "object/ui/enemy_info_ui_layer.h"

namespace event_test
{
	RotatingCube::RotatingCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/bricks_cube.fbx")
	{
		m_ui_layer = CreateSPtr<EnemyInfoUILayer>();
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
		m_widget_component = CreateSPtr<WidgetComponent>(m_ui_layer);
	}

	bool RotatingCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_ui_layer->SetRotatingCube(std::static_pointer_cast<RotatingCube>(shared_from_this()));
		ret &= AttachComponent(m_rotating_component);
		ret &= AttachComponent(m_widget_component);
		m_widget_component->SetLocalPosition(Vec3(0.0f, 90.0f, 0.0f));
		m_widget_component->SetSize(Vec2(200.0f, 60.0f));
		//m_rotating_component->SetRotatingRate(Vec3(0.0f, 0.0f, 0.0f));

		const auto& render_camera = CreateSPtr<RenderCameraComponent>();
		render_camera->SetActive();
		//ret &= AttachComponent(render_camera);


		RegisterReceiveMessage(HashCode("change rotating speed"));
		RegisterReceiveMessage(HashCode("move object"));
		RegisterReceiveMessage(HashCode("testmove"));
		
		return ret;
	}

	void RotatingCube::Shutdown()
	{
		m_rotating_component = nullptr;
		m_widget_component = nullptr;
		m_ui_layer = nullptr;
		StaticMeshActor::Shutdown();
	}

	void RotatingCube::Update(float delta_time)
	{
		if (m_rotating_y_speed != m_rotating_component->GetRotatingRate().y)
		{
			m_rotating_y_speed = m_rotating_component->GetRotatingRate().y;
			if (m_speed_change_function != nullptr)
				m_speed_change_function(m_rotating_y_speed);
		}
		
		//--------방햑벡터 보내서 확인하는 작업
		//if (m_network_vec != Vec3(0.0f, 0.0f, 0.0f))
		//{
		//	Vec3 new_pos = GetPosition() + m_network_vec*m_nw_speed*delta_time;
		//	LOG_INFO(new_pos);
		//	SetPosition(new_pos);
		//}
		
	}

	void RotatingCube::ExecuteMessage(const SPtr<MessageEventInfo>& message)
	{
	}

	void RotatingCube::ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message)
	{
	}
}