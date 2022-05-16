#pragma once
#include "client/renderer/renderitem/core/render_item.h"

namespace client_fw
{
	class Mesh;
	class MeshComponent;
	class StaticMeshComponent;
	struct RSInstanceData;
	struct RSSkeletalData;
	struct MeshesInstanceDrawInfo;

	enum class eRenderLevelType;

	struct StaticMeshData
	{
		SPtr<Mesh> mesh;
		std::vector<SPtr<StaticMeshComponent>> mesh_comps;
	};

	class MeshRenderItem : public RenderItem
	{
	public:
		MeshRenderItem(const std::string& owner_shader_name);
		virtual ~MeshRenderItem() {}

		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& draw_function) const = 0;

		virtual void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) = 0;
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) = 0;
	};

	class StaticMeshRenderItem final : public MeshRenderItem
	{
	public:
		StaticMeshRenderItem(const std::string& owner_shader_name);
		virtual ~StaticMeshRenderItem();

		virtual void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types) override;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
			std::function<void()>&& draw_function) const override;

		virtual void RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) override;
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) override;

	private:
		std::vector<SPtr<StaticMeshData>> m_mesh_data;
		std::map<std::string, SPtr<StaticMeshData>> m_mesh_data_map;

		std::map<eRenderLevelType, std::vector<RSInstanceData>> m_meshes_instance_data;
	};
}
