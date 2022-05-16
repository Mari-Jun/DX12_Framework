#include "stdafx.h"
#include "client/renderer/renderitem/ui_render_item.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/ui_frame_resource.h"
#include "client/renderer/core/render.h"
#include "client/asset/primitive/primitive.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/object/ui/core/user_interface.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	UIRenderItem::UIRenderItem(const std::string& owner_shader_name)
		: RenderItem(owner_shader_name)
	{
	}

	UIRenderItem::~UIRenderItem()
	{
	}

	void UIRenderItem::Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types)
	{
		const auto& frame_resources = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resources)
		{
			for (eRenderLevelType level_type : level_types)
				frame->CreateUserInterfaceFrameResource(device, m_owner_shader_name, level_type);
		}
	}

	void UIRenderItem::Shutdown()
	{
	}

	void UIRenderItem::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		Vec2 window_size = Render::GetWindowSize();

		const auto& ui_manager = UserInterfaceManager::GetUIManager();

		for (const auto& ui_layer : ui_manager.GetUILayers())
		{
			if (ui_layer->GetUILayerState() == eUILayerState::kActive)
			{
				const auto& user_interfaces = ui_layer->GetUserInterfaces();

				for (const auto& ui : user_interfaces)
				{
					if (ui->IsVisible())
					{
						for (const auto& ui_texture : ui->GetVisibleTextures())
						{
							if (ui_texture == nullptr)
								LOG_WARN("Could not find ui texture : {0}", ui->GetName());
							else
							{
								Vec2 new_position = (ui->GetPivotPosition() + ui_texture->GetPosition()) * Vec2(1.0f, -1.0f) +
									Vec2(window_size.x * -0.5f, window_size.y * 0.5f);
								INT resource_index = -1;

								if (ui_texture->GetTexture() != nullptr)
									resource_index = ui_texture->GetTexture()->GetResourceIndex();

								m_vertices[level_type].emplace_back(UIVertex(new_position, ui_texture->GetSize(), resource_index,
									ui_texture->GetBrushColor(), ui_texture->GetCoordinate(), ui_texture->GetTilling()));
							}
						}
					}
				}
			}
		}

	}

	void UIRenderItem::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		const auto& ui_resource = FrameResourceManager::GetManager().
			GetCurrentFrameResource()->GetUserInterfaceFrameResource(m_owner_shader_name, level_type);

		m_num_of_draw_ui_data = static_cast<UINT>(m_vertices[level_type].size());

		if (m_num_of_draw_ui_data > 0)
		{
			UINT ui_primitive_size = ui_resource->GetSizeOfUIPrimitive();
			bool is_need_resource_create = false;

			while (ui_primitive_size <= m_num_of_draw_ui_data)
			{
				ui_primitive_size = static_cast<UINT>(roundf(static_cast<float>(ui_primitive_size) * 1.5f));
				is_need_resource_create = true;
			}

			if (is_need_resource_create)
			{
				ui_resource->GetUserInterfacePrimitive()->Update(device, ui_primitive_size);
				ui_resource->SetSizeOfUIPrimitive(ui_primitive_size);
			}

			ui_resource->GetUserInterfacePrimitive()->UpdateVertices(m_vertices[level_type]);
			m_vertices[level_type].clear();
		}
	}

	void UIRenderItem::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type, std::function<void()>&& draw_function)
	{
		if (m_num_of_draw_ui_data)
		{
			const auto& ui_resource = FrameResourceManager::GetManager().
				GetCurrentFrameResource()->GetUserInterfaceFrameResource(m_owner_shader_name, level_type);
			draw_function();
			ui_resource->GetUserInterfacePrimitive()->PreDraw(command_list);
			ui_resource->GetUserInterfacePrimitive()->Draw(command_list, m_num_of_draw_ui_data);
		}
	}
}
