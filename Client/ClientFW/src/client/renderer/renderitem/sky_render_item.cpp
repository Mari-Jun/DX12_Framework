#include "stdafx.h"
#include "client/renderer/renderitem/sky_render_item.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/asset/mesh/cube_mesh.h"
#include "client/asset/texture/texture.h"
#include "client/object/component/sky/sky_cube_component.h"
#include "client/object/component/sky/sky_sphere_component.h"

namespace client_fw
{
	SkyRenderItem::SkyRenderItem(const std::string& owner_shader_name)
		: RenderItem(owner_shader_name)
	{
		m_sky_cube_mesh = CreateSPtr<SkyCubeMesh>();
	}

	SkyRenderItem::~SkyRenderItem()
	{
	}

	void SkyRenderItem::Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types)
	{
		RenderResourceManager::GetRenderResourceManager().RegisterPrimitive(m_sky_cube_mesh);
		const auto& frame_resource = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resource)
		{
			for (eRenderLevelType level_type : level_types)
				frame->CreateSkyFrameResource(device, m_owner_shader_name, level_type);
		}
	}

	void SkyRenderItem::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
	}

	void SkyRenderItem::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		if (m_sky_components.empty() == false)
		{
			const auto& last_sky = *m_sky_components.rbegin();

			m_draw_sky_type = last_sky->GetSkyType();

			const auto& sky_frame_resource = FrameResourceManager::GetManager().
				GetCurrentFrameResource()->GetSkyFrameResource(m_owner_shader_name, level_type);

			switch (m_draw_sky_type)
			{
			case client_fw::eSkyType::kCube:
			{
				const auto& sky_cube_component = std::static_pointer_cast<SkyCubeComponent>(last_sky);
				if (sky_cube_component->GetCubeMapTexture() != nullptr)
				{
					UINT index = sky_cube_component->GetCubeMapTexture()->GetResourceIndex();
					sky_frame_resource->GetCubeTextureIndexData()->CopyData(0,
						RSCubeTextureIndexData{ index });
				}
				break;
			}
			case client_fw::eSkyType::kSphere:
			{
				const auto& sky_sphere = std::static_pointer_cast<SkySphereComponent>(last_sky);
				sky_frame_resource->GetSkySpherePrimitive()->UpdateVertices(
					{ SkySphereVertex{sky_sphere->GetCenterColor(), sky_sphere->GetApexColor()},
					SkySphereVertex{sky_sphere->GetCenterColor(), sky_sphere->GetApexColor()} }
				);
				break;
			}
			default:
				break;
			}
		}
	}

	void SkyRenderItem::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
		std::function<void()>&& cube_draw_function,
		std::function<void()>&& sphere_draw_function)
	{
		if (m_sky_components.empty() == false)
		{
			const auto& sky_frame_resource = FrameResourceManager::GetManager().
				GetCurrentFrameResource()->GetSkyFrameResource(m_owner_shader_name, level_type);

			switch (m_draw_sky_type)
			{
			case client_fw::eSkyType::kCube:
			{
				command_list->SetGraphicsRootConstantBufferView(0, sky_frame_resource->GetCubeTextureIndexData()->GetResource()->GetGPUVirtualAddress());

				cube_draw_function();
				m_sky_cube_mesh->Draw(command_list);
				break;
			}
			case client_fw::eSkyType::kSphere:
			{
				sphere_draw_function();
				sky_frame_resource->GetSkySpherePrimitive()->PreDraw(command_list);
				sky_frame_resource->GetSkySpherePrimitive()->Draw(command_list, 2);
				break;
			}
			default:
				break;
			}
		}
	}

	void SkyRenderItem::RegisterSkyComponent(const SPtr<SkyComponent>& sky_component)
	{
		m_sky_components.push_back(sky_component);
	}

	void SkyRenderItem::UnregisterSkyComponent(const SPtr<SkyComponent>& sky_component)
	{
		auto iter = std::find(m_sky_components.begin(), m_sky_components.end(), sky_component);
		if (iter != m_sky_components.end())
		{
			std::iter_swap(iter, m_sky_components.end() - 1);
			m_sky_components.pop_back();
		}
	}
}
