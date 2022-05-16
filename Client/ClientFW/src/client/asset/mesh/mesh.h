#pragma once
#include "client/asset/core/asset.h"
#include "client/asset/primitive/primitive.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class MeshComponent;
	class Material;
	template<class T> class UploadBuffer;

	// Mesh에 있는 Material별로 한번에 그리는것이 효율적이기 때문에 존재하는 구조체이다.
	// count는 정점의 수, start_location은 정점의 시작 위치 index를 표현한다.
	struct MeshVertexInfo
	{
		UINT count;
		UINT start_location;
	};

	struct RSMaterialIndexData
	{
		UINT index;
	};

	class Mesh : public Asset, public Primitive
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual void Shutdown() override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const = 0;

		virtual void CreateDataForLodMesh(UINT lod);

	protected:
		bool m_is_visible = false;
		UINT m_lod_count = 0;

		std::vector<SPtr<VertexInfo>> m_vertex_infos;
		std::vector<SPtr<IndexInfo>> m_index_infos;

		BOrientedBox m_oriented_box;

		bool m_is_draw_index = false;

	public:
		bool IsVisible() const { return m_is_visible; }
		void SetVisible(bool visible) { m_is_visible = visible; }
		UINT GetLODCount() const { return m_lod_count; }
		const SPtr<VertexInfo>& GetVertexInfo(UINT lod) const { return m_vertex_infos.at(lod); }
		const SPtr<IndexInfo>& GetIndexInfo(UINT lod) const { return m_index_infos.at(lod); }
		const BOrientedBox& GetOrientedBox() const { return m_oriented_box; }
		void SetOrientBox(const BOrientedBox& oriented_box) { m_oriented_box = oriented_box; }
		void SetDrawIndex(bool value) { m_is_draw_index = value; }
		bool IsDrawIndex() const { return m_is_draw_index; }
 
	protected:
		std::vector<std::vector<MeshVertexInfo>> m_mesh_vertex_info;
		std::vector<std::vector<SPtr<Material>>> m_materials;
		std::vector<UPtr<UploadBuffer<RSMaterialIndexData>>> m_material_index_data;

	public:
		virtual void AddMeshVertexInfo(UINT lod, MeshVertexInfo&& info);
		void AddMaterial(UINT lod, SPtr<Material>&& material) { m_materials.at(lod).emplace_back(std::move(material)); }
		const std::vector<SPtr<Material>> GetMaterials(UINT lod) const { return m_materials.at(lod); }
	};

	class TextureLightVertex;
	class KDTree;

	class StaticMesh : public Mesh
	{
	public:
		StaticMesh() = default;
		virtual ~StaticMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const override;
		virtual void DrawForShadow(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const override;

		SPtr<KDTree> m_bounding_tree;

	public:
		TextureLightVertex* GetVertices(UINT lod) const { return m_vertex_infos.at(lod)->GetVertices<TextureLightVertex>(); }
		UINT GetVertexCount(UINT lod) const { return m_vertex_infos.at(lod)->GetVertexCount<TextureLightVertex>(); }
		const SPtr<KDTree>& GetBoundingTree() const { return m_bounding_tree; }
		void SetBoundingTree(SPtr<KDTree>&& tree) { m_bounding_tree = std::move(tree); }
	
	};

	struct BoneData
	{
		std::vector<std::string> bone_names;
		std::vector<Mat4> bone_offsets; 
		std::vector<IVec4> bone_indices; //BoneVertex
		std::vector<Vec4> bone_weights; //BoneVertex
		std::vector<BOrientedBox> oriented_boxes;
	};

	class SkeletalMesh : public Mesh
	{
	public:
		SkeletalMesh() = default;
		virtual ~SkeletalMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const override;
		virtual void DrawForShadow(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const override;

	private:
		SPtr<Skeleton> m_skeleton;
		SPtr<BoneData> m_bone_data;

	public:
		const SPtr<Skeleton>& GetSkeleton() { return m_skeleton; }
		void SetSkeleton(SPtr<Skeleton>& skeleton) { m_skeleton = skeleton; }

		const SPtr<BoneData>& GetBoneData() { return m_bone_data; }
		void SetBoneData(const SPtr<BoneData>& bone_data) { m_bone_data = bone_data; }
	
		UINT GetBoneCount() { return static_cast<UINT>(m_bone_data->bone_names.size()); }
	};
}
