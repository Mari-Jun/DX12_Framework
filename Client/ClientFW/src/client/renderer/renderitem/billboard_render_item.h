#pragma once
#include "client/renderer/renderitem/core/render_item.h"

namespace client_fw
{
	class BillboardComponent;
	class TextureBillboardComponent;
	class MaterialBillboardComponent;

	template<class VertexType>
	class UploadPrimitive;
	class BillboardVertex;

	class BillboardRenderItem : public RenderItem
	{
	public:
		BillboardRenderItem(const std::string& owner_shader_name);
		virtual ~BillboardRenderItem();

		void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types) override;

		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& draw_function, std::function<void()>&& fix_up_draw_function) = 0;

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) {}
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) {}

	protected:
		std::map<eRenderLevelType, std::vector<BillboardVertex>> m_vertices;
	};
	
	class TextureBillboardRenderItem final : public BillboardRenderItem
	{
	public:
		TextureBillboardRenderItem(const std::string& owner_shader_name);
		virtual ~TextureBillboardRenderItem();

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& draw_function, std::function<void()>&& fix_up_draw_function) override;

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;

	private:
		std::vector<SPtr<TextureBillboardComponent>> m_billboard_components;
	};

	class MaterialBillboardRenderItem final : public BillboardRenderItem
	{
	public:
		MaterialBillboardRenderItem(const std::string& owner_shader_name);
		virtual ~MaterialBillboardRenderItem();

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& draw_function, std::function<void()>&& fix_up_draw_function) override;

		virtual void RegisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;

	private:
		std::vector<SPtr<MaterialBillboardComponent>> m_billboard_components;
	};
}



