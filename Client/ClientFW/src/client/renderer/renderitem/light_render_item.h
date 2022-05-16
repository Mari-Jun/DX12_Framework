#pragma once
#include "client/renderer/renderitem/core/render_item.h"

namespace client_fw
{
	struct RSLocalLightInstanceData;
	class LocalLightComponent;

	class LightRenderItem : public RenderItem
	{
	public:
		LightRenderItem(const std::string& owner_shader_name);
		virtual ~LightRenderItem();

		void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types) override;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list,
			eRenderLevelType level_type, std::function<void()>&& draw_function) const = 0;

		virtual void RegisterLightComponent(const SPtr<LocalLightComponent>& light_comp);
		virtual void UnregisterLightComponent(const SPtr<LocalLightComponent>& light_comp);

	protected:
		std::vector<SPtr<LocalLightComponent>> m_light_components;

		std::map<eRenderLevelType, std::vector<RSLocalLightInstanceData>> m_local_light_instance_data;
	};

	class PointLightRenderItem : public LightRenderItem
	{
	public:
		PointLightRenderItem(const std::string& owner_shader_name);
		virtual ~PointLightRenderItem() = default;

		virtual void Draw(ID3D12GraphicsCommandList* command_list, 
			eRenderLevelType level_type, std::function<void()>&& draw_function) const override;
	};

	class SpotLightRenderItem : public LightRenderItem
	{
	public:
		SpotLightRenderItem(const std::string& owner_shader_name);
		virtual ~SpotLightRenderItem() = default;

		virtual void Draw(ID3D12GraphicsCommandList* command_list, 
			eRenderLevelType level_type, std::function<void()>&& draw_function) const override;
	};



}
