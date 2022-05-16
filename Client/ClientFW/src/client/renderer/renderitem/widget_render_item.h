#pragma once
#include "client/renderer/renderitem/core/render_item.h"

namespace client_fw
{
	class WidgetComponent;

	class PivotWidgetVertex;
	class WorldWidgetVertex;
	enum class eWidgetSpaceType;

	class WidgetRenderItem final : public RenderItem
	{
	public:
		WidgetRenderItem(const std::string& owner_shader_name);
		virtual ~WidgetRenderItem();

		void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types);

		void Update(ID3D12Device* device, eRenderLevelType level_type);
		void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type);
		void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& world_function, std::function<void()>&& billboard_function,
			std::function<void()>&& fix_up_function);

		void RegisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);
		void UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);

	private:
		void UpdateWorldWidgets(ID3D12Device* device, eRenderLevelType level_type);
		void UpdatePivotWidgets(ID3D12Device* device, eRenderLevelType level_type);

	private:
		std::vector<SPtr<WidgetComponent>> m_world_widget_components;
		std::vector<SPtr<WidgetComponent>> m_widget_components;

		std::map<eRenderLevelType, std::vector<WorldWidgetVertex>> m_world_widget_vertices;
		std::map<eRenderLevelType, std::vector<PivotWidgetVertex>> m_pivot_widget_vertices;
	};
}



