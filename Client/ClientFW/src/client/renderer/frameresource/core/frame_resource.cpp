#include "stdafx.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/render_resource_frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/renderer/frameresource/shadow_frame_resource.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/renderer/frameresource/mesh_frame_resource.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/renderer/frameresource/widget_frame_resource.h"
#include "client/renderer/frameresource/ui_frame_resource.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	bool operator<(const ResourceOwner& owner1, const ResourceOwner& owner2)
	{
		if (owner1.owner_shader_name == owner2.owner_shader_name)
			return owner1.render_level_type < owner2.render_level_type;
		else
			return owner1.owner_shader_name < owner2.owner_shader_name;
	}

	FrameResource::FrameResource()
	{
		m_render_resource_frame_resource = CreateUPtr<RenderResourceFrameResource>();
		m_camera_frame_resource = CreateUPtr<CameraFrameResource>();
		m_light_frame_resource = CreateUPtr<LightFrameResource>();
		m_shadow_frame_resource = CreateUPtr<ShadowFrameResource>();
	}

	FrameResource::~FrameResource()
	{
	}

	bool FrameResource::Initialize(ID3D12Device* device)
	{
		if (FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_command_allocator.GetAddressOf()))))
		{
			LOG_ERROR("Could not create command allocator");
			return false;
		}

		m_render_resource_frame_resource->Initialize(device);
		m_camera_frame_resource->Initialize(device);
		m_light_frame_resource->Initialize(device);
		m_shadow_frame_resource->Initialize(device);

		return true;
	}

	void FrameResource::Shutdown()
	{
		for (const auto& [shader_name, frame_resource] : m_local_light_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_ui_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_widget_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_billboard_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_static_mesh_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_skeletal_mesh_frame_resource)
			frame_resource->Shutdown();
		for (const auto& [shader_name, frame_resource] : m_sky_frame_resource)
			frame_resource->Shutdown();
		m_shadow_frame_resource->Shutdown();
		m_light_frame_resource->Shutdown();
		m_camera_frame_resource->Shutdown();
		m_render_resource_frame_resource->Shutdown();
	}

	void FrameResource::CreateSkyFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_sky_frame_resource.emplace(owner, CreateUPtr<SkyFrameResource>());
		m_sky_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateStaticMeshFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_static_mesh_frame_resource.emplace(owner, CreateUPtr<StaticMeshFrameResource>());
		m_static_mesh_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateSkeletalMeshFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_skeletal_mesh_frame_resource.emplace(owner, CreateUPtr<SkeletalMeshFrameResource>());
		m_skeletal_mesh_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateBillboardFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_billboard_frame_resource.emplace(owner, CreateUPtr<BillboardFrameResource>());
		m_billboard_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateWidgetFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_widget_frame_resource.emplace(owner, CreateUPtr<WidgetFrameResource>());
		m_widget_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateLocalLightFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_local_light_frame_resource.emplace(owner, CreateUPtr<LocalLightFrameResource>());
		m_local_light_frame_resource[owner]->Initialize(device);
	}

	void FrameResource::CreateUserInterfaceFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type)
	{
		ResourceOwner owner{ shader_name, level_type };
		m_ui_frame_resource.emplace(owner, CreateUPtr<UserInterfaceFrameResource>());
		m_ui_frame_resource[owner]->Initialize(device);
	}

}
