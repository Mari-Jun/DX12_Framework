#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/component/render/widget_component.h>

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
}
