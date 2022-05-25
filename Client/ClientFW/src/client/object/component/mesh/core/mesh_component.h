#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class Mesh;
	struct VisualTreeNode;

	class MeshComponent : public RenderComponent
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual void Shutdown() override;

		virtual void UpdateWorldMatrix() override;

	protected:
		virtual void UpdateLevelOfDetail(const Vec3& eye) override;
		virtual void UpdateLevelOfDetailForShadow(const Vec3& eye) override;
		virtual void ResetLevelOfDetailForShadow() override;
		virtual void UpdateOrientedBox() override;

	protected:
		virtual bool RegisterToRenderSystem() override;
		virtual void UnregisterFromRenderSystem() override;
		virtual void RegisterToVisualOctree() override;
		virtual void UnregisterFromVisualOctree() override;

	protected:
		SPtr<Mesh> m_mesh;
		float m_max_extent = 0;
		UINT m_level_of_detail = 0;
		UINT m_level_of_detail_for_shadow = 0;

	private:
		Mat4 m_world_transpose_matrix;
		Mat4 m_world_inverse_matrix;

	public:
		virtual bool SetMesh(const std::string& file_path) = 0; 
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		float GetMaxExtent() const { return m_max_extent; }
		bool IsUseLevelOfDetail() const;
		UINT GetLevelOfDetail() const { return m_level_of_detail; }
		void SetLevelOfDetail(UINT lod);
		UINT GetLevelOfDetailForShadow() const { return m_level_of_detail_for_shadow; }
		const Mat4& GetWorldTransposeMatrix() const { return m_world_transpose_matrix; }
		const Mat4& GetWorldInverseMatrix() const { return m_world_inverse_matrix; }


	public:
		//
		//  ↓ 사용자용 함수 X
		//
		virtual void SetVisiblity(bool value) override;
	};
}


