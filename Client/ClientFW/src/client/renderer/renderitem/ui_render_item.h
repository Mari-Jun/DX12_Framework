#pragma once
#include "client/renderer/renderitem/core/render_item.h"

namespace client_fw
{
	class UserInterface;

	class UIVertex;

	class UIRenderItem final : public RenderItem
	{
	public:
		UIRenderItem(const std::string& owner_shader_name);
		virtual ~UIRenderItem();

		void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types);
		void Shutdown();

		void Update(ID3D12Device* device, eRenderLevelType level_type);
		void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type);
		void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type, std::function<void()>&& draw_function);

	private:
		UINT m_num_of_draw_ui_data = 0;

		std::map<eRenderLevelType, std::vector<UIVertex>> m_vertices;
	};
}



