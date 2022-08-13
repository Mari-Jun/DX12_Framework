#include "stdafx.h"
#include "client/renderer/renderitem/light_render_item.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/object/component/light/point_light_component.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	LightRenderItem::LightRenderItem(const std::string& owner_shader_name)
		: RenderItem(owner_shader_name)
	{
	}

	LightRenderItem::~LightRenderItem()
	{
	}

	void LightRenderItem::Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types)
	{
		const auto& frame_resources = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resources)
		{
			for (eRenderLevelType level_type : level_types)
				frame->CreateLocalLightFrameResource(device, m_owner_shader_name, level_type);
		}
	}

	void LightRenderItem::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		LocalLightInstanceDrawInfo instance_info;
		//카메라마다의 시작 위치 저장
		instance_info.start_index = static_cast<UINT>(m_local_light_instance_data[level_type].size());

		for (const auto& light : m_light_components)
		{
			if (light->IsVisible())
			{
				const auto& transpos_matrix = light->GetWorldTransposeMatrix();

				m_local_light_instance_data[level_type].push_back(
					RSLocalLightInstanceData{ transpos_matrix, transpos_matrix,
					light->GetLightManagerRegisteredIndex() }
				);

				light->SetVisiblity(false);
			}
		}

		instance_info.num_of_instance_data = static_cast<UINT>(m_local_light_instance_data[level_type].size() - instance_info.start_index);

		const auto& light_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetLocalLightFrameResource(m_owner_shader_name, level_type);
		light_resource->AddLocalLightDrawInfo(std::move(instance_info));
	}

	void LightRenderItem::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		const auto& light_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetLocalLightFrameResource(m_owner_shader_name, level_type);

		UINT new_size = static_cast<UINT>(m_local_light_instance_data[level_type].size());
		if (new_size > 0)
		{
			UINT light_instance_size = light_resource->GetSizeOfLightInstanceData();
			bool is_need_resource_create = false;

			while (light_instance_size <= new_size)
			{
				light_instance_size = static_cast<UINT>(roundf(static_cast<float>(light_instance_size) * 1.5f));
				is_need_resource_create = true;
			}

			if (is_need_resource_create)
			{
				light_resource->GetLightInstanceData()->CreateResource(device, light_instance_size);
				light_resource->SetSizeOfLightInstanceData(light_instance_size);
			}

			light_resource->GetLightInstanceData()->CopyVectorData(m_local_light_instance_data[level_type]);

			m_local_light_instance_data[level_type].clear();
		}
	}

	void LightRenderItem::RegisterLightComponent(const SPtr<LocalLightComponent>& light_comp)
	{
		light_comp->SetRenderItemIndex(static_cast<UINT>(m_light_components.size()));
		m_light_components.push_back(light_comp);
	}

	void LightRenderItem::UnregisterLightComponent(const SPtr<LocalLightComponent>& light_comp)
	{
		UINT index = light_comp->GetRenderItemIndex();

		std::iter_swap(m_light_components.begin() + index, m_light_components.end() - 1);
		m_light_components[index]->SetRenderItemIndex(index);
		m_light_components.pop_back();
	}

	PointLightRenderItem::PointLightRenderItem(const std::string& owner_shader_name)
		: LightRenderItem(owner_shader_name)
	{
	}

	void PointLightRenderItem::Draw(ID3D12GraphicsCommandList* command_list,
		eRenderLevelType level_type, std::function<void()>&& draw_function) const
	{
		const auto& light_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetLocalLightFrameResource(m_owner_shader_name, level_type);

		LocalLightInstanceDrawInfo instance_info = light_resource->GetLocalLightDrawInfo();

		if (instance_info.num_of_instance_data > 0)
		{
			const auto& instance_data = light_resource->GetLightInstanceData();

			draw_function();
			command_list->SetGraphicsRootShaderResourceView(1, instance_data->GetResource()->GetGPUVirtualAddress() +
				instance_info.start_index * instance_data->GetByteSize());
			command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
			command_list->DrawInstanced(2, instance_info.num_of_instance_data, 0, 0);
		}

	}

	SpotLightRenderItem::SpotLightRenderItem(const std::string& owner_shader_name)
		: LightRenderItem(owner_shader_name)
	{
	}

	void SpotLightRenderItem::Draw(ID3D12GraphicsCommandList* command_list,
		eRenderLevelType level_type, std::function<void()>&& draw_function) const
	{
		const auto& light_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetLocalLightFrameResource(m_owner_shader_name, level_type);

		LocalLightInstanceDrawInfo instance_info = light_resource->GetLocalLightDrawInfo();

		if (instance_info.num_of_instance_data > 0)
		{
			const auto& instance_data = light_resource->GetLightInstanceData();

			draw_function();
			command_list->SetGraphicsRootShaderResourceView(1, instance_data->GetResource()->GetGPUVirtualAddress() +
				instance_info.start_index * instance_data->GetByteSize());
			command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
			command_list->DrawInstanced(1, instance_info.num_of_instance_data, 0, 0);
		}
	}
}
