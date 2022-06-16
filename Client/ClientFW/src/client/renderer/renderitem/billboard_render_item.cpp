#include "stdafx.h"
#include "client/renderer/renderitem/billboard_render_item.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/object/component/render/texture_billboard_component.h"
#include "client/object/component/render/material_billboard_component.h"
#include "client/asset/primitive/primitive.h"
#include "client/asset/texture/texture.h"
#include "client/asset/material/material.h"
#include "client/util/octree/octree.h"

namespace client_fw
{
	BillboardRenderItem::BillboardRenderItem(const std::string& owner_shader_name)
		: RenderItem(owner_shader_name)
	{
	}

	BillboardRenderItem::~BillboardRenderItem()
	{
	}

	void BillboardRenderItem::Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types)
	{
		const auto& frame_resources = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resources)
		{
			for (eRenderLevelType level_type : level_types)
				frame->CreateBillboardFrameResource(device, m_owner_shader_name, level_type);
		}
	}

	TextureBillboardRenderItem::TextureBillboardRenderItem(const std::string& owner_shader_name)
		: BillboardRenderItem(owner_shader_name)
	{
	}

	TextureBillboardRenderItem::~TextureBillboardRenderItem()
	{
	}

	void TextureBillboardRenderItem::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		std::vector<BillboardVertex> vertices;
		std::vector<BillboardVertex> fix_up_vertices;

		for (const auto& bb_comp : m_billboard_components)
		{
			if (bb_comp->IsVisible() && bb_comp->GetTexture() != nullptr)
			{
				BillboardVertex vertex(BillboardVertex(bb_comp->GetWorldPosition(),
					bb_comp->GetSize(), bb_comp->GetTexture()->GetResourceIndex(),
					bb_comp->GetOffset(), bb_comp->GetTilling()));

				if (bb_comp->IsFixUpVector())
					fix_up_vertices.emplace_back(std::move(vertex));
				else
					vertices.emplace_back(std::move(vertex));

				bb_comp->SetVisiblity(false);
			}
		}

		const auto& billboard_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);

		BillboardDrawInfo info;
		info.start_index = static_cast<UINT>(m_vertices[level_type].size());
		info.num_of_draw_data = static_cast<UINT>(vertices.size());
		std::move(vertices.begin(), vertices.end(), std::back_inserter(m_vertices[level_type]));
		info.fix_up_start_index = static_cast<UINT>(m_vertices[level_type].size());
		info.num_of_draw_fix_up_data = static_cast<UINT>(fix_up_vertices.size());
		std::move(fix_up_vertices.begin(), fix_up_vertices.end(), std::back_inserter(m_vertices[level_type]));

		billboard_resource->AddBillboardDrawInfo(std::move(info));
	}

	void TextureBillboardRenderItem::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		UINT new_size = static_cast<UINT>(m_vertices[level_type].size());
		if (new_size > 0)
		{
			const auto& billboard_resource = FrameResourceManager::GetManager().
				GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);
			
			UINT primitive_size = billboard_resource->GetSizeOfBillboardPrimitive();
			bool is_need_resource_create = false;

			while (primitive_size <= new_size)
			{
				primitive_size = static_cast<UINT>(roundf(static_cast<float>(primitive_size) * 1.5f));
				is_need_resource_create = true;
			}

			if (is_need_resource_create)
			{
				billboard_resource->GetBillboardPrimitive()->Update(device, primitive_size);
				billboard_resource->SetSizeOfBillboardPrimitive(primitive_size);
			}

			billboard_resource->GetBillboardPrimitive()->UpdateVertices(m_vertices[level_type]);
			m_vertices[level_type].clear();
		}
	}

	void TextureBillboardRenderItem::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
		std::function<void()>&& draw_function, std::function<void()>&& fix_up_draw_function)
	{
		const auto& billboard_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);

		BillboardDrawInfo info = billboard_resource->GetBillboardDrawInfo();

		if (info.num_of_draw_data + info.num_of_draw_fix_up_data != 0)
		{
			billboard_resource->GetBillboardPrimitive()->PreDraw(command_list);
			if (info.num_of_draw_data != 0)
			{
				draw_function();
				billboard_resource->GetBillboardPrimitive()->Draw(command_list, info.num_of_draw_data, info.start_index);
			}
			if (info.num_of_draw_fix_up_data != 0)
			{
				fix_up_draw_function();
				billboard_resource->GetBillboardPrimitive()->Draw(command_list, info.num_of_draw_fix_up_data, info.fix_up_start_index);
			}
		}
	}

	void TextureBillboardRenderItem::RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		if (bb_comp->GetBillboardType() == eBillboardType::kTexture)
		{
			bb_comp->SetRenderItemIndex(static_cast<UINT>(m_billboard_components.size()));
			m_billboard_components.push_back(std::static_pointer_cast<TextureBillboardComponent>(bb_comp));
		}
		else
		{
			LOG_WARN("Could not register {0} to texture billboard render item", bb_comp->GetName());
		}
	}

	void TextureBillboardRenderItem::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		UINT index = bb_comp->GetRenderItemIndex();

		if (bb_comp->GetBillboardType() == eBillboardType::kTexture)
		{
			std::iter_swap(m_billboard_components.begin() + index, m_billboard_components.end() - 1);
			m_billboard_components[index]->SetRenderItemIndex(index);
			m_billboard_components.pop_back();
		}
	}

	MaterialBillboardRenderItem::MaterialBillboardRenderItem(const std::string& owner_shader_name)
		: BillboardRenderItem(owner_shader_name)
	{
	}

	MaterialBillboardRenderItem::~MaterialBillboardRenderItem()
	{
	}

	void MaterialBillboardRenderItem::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		std::vector<BillboardVertex> vertices;
		std::vector<BillboardVertex> fix_up_vertices;

		for (const auto& bb_comp : m_billboard_components)
		{
			if (bb_comp->IsVisible() && bb_comp->GetMaterial() != nullptr)
			{
				BillboardVertex vertex(BillboardVertex(bb_comp->GetWorldPosition(),
					bb_comp->GetSize(), bb_comp->GetMaterial()->GetResourceIndex(),
					Vec2(0.0f, 0.0f), Vec2(1.f, 1.f)));

				if (bb_comp->IsFixUpVector())
					fix_up_vertices.emplace_back(std::move(vertex));
				else
					vertices.emplace_back(std::move(vertex));

				bb_comp->SetVisiblity(false);
			}
		}

		const auto& billboard_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);

		BillboardDrawInfo info;
		info.start_index = static_cast<UINT>(m_vertices[level_type].size());
		info.num_of_draw_data = static_cast<UINT>(vertices.size());
		std::move(vertices.begin(), vertices.end(), std::back_inserter(m_vertices[level_type]));
		info.fix_up_start_index = static_cast<UINT>(m_vertices[level_type].size());
		info.num_of_draw_fix_up_data = static_cast<UINT>(fix_up_vertices.size());
		std::move(fix_up_vertices.begin(), fix_up_vertices.end(), std::back_inserter(m_vertices[level_type]));

		billboard_resource->AddBillboardDrawInfo(std::move(info));
	}

	void MaterialBillboardRenderItem::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		UINT new_size = static_cast<UINT>(m_vertices[level_type].size());
		if (new_size > 0)
		{
			const auto& billboard_resource = FrameResourceManager::GetManager().
				GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);

			UINT primitive_size = billboard_resource->GetSizeOfBillboardPrimitive();
			bool is_need_resource_create = false;

			while (primitive_size <= new_size)
			{
				primitive_size = static_cast<UINT>(roundf(static_cast<float>(primitive_size) * 1.5f));
				is_need_resource_create = true;
			}

			if (is_need_resource_create)
			{
				billboard_resource->GetBillboardPrimitive()->Update(device, primitive_size);
				billboard_resource->SetSizeOfBillboardPrimitive(primitive_size);
			}

			billboard_resource->GetBillboardPrimitive()->UpdateVertices(m_vertices[level_type]);
			m_vertices[level_type].clear();
		}
	}

	void MaterialBillboardRenderItem::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
		std::function<void()>&& draw_function, std::function<void()>&& fix_up_draw_function)
	{
		const auto& billboard_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetBillboardFrameResource(m_owner_shader_name, level_type);

		BillboardDrawInfo info = billboard_resource->GetBillboardDrawInfo();

		if (info.num_of_draw_data + info.num_of_draw_fix_up_data != 0)
		{
			billboard_resource->GetBillboardPrimitive()->PreDraw(command_list);
			if (info.num_of_draw_data != 0)
			{
				draw_function();
				billboard_resource->GetBillboardPrimitive()->Draw(command_list, info.num_of_draw_data, info.start_index);
			}
			if (info.num_of_draw_fix_up_data != 0)
			{
				fix_up_draw_function();
				billboard_resource->GetBillboardPrimitive()->Draw(command_list, info.num_of_draw_fix_up_data, info.fix_up_start_index);
			}
		}
	}

	void MaterialBillboardRenderItem::RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		if (bb_comp->GetBillboardType() == eBillboardType::kMaterial)
		{
			bb_comp->SetRenderItemIndex(static_cast<UINT>(m_billboard_components.size()));
			m_billboard_components.push_back(std::static_pointer_cast<MaterialBillboardComponent>(bb_comp));
		}
		else
		{
			LOG_WARN("Could not register {0} to material billboard render item", bb_comp->GetName());
		}
	}

	void MaterialBillboardRenderItem::UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp)
	{
		UINT index = bb_comp->GetRenderItemIndex();

		if (bb_comp->GetBillboardType() == eBillboardType::kMaterial)
		{
			std::iter_swap(m_billboard_components.begin() + index, m_billboard_components.end() - 1);
			m_billboard_components[index]->SetRenderItemIndex(index);
			m_billboard_components.pop_back();
		}
	}
}
