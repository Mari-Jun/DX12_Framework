#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/util/octree/octree_manager.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name, const std::string& draw_shader_name)
		: RenderComponent(name, 20, eRenderType::kMesh, draw_shader_name)
	{
	}

	void MeshComponent::Shutdown()
	{
		m_mesh = nullptr;
	}

	void MeshComponent::UpdateWorldMatrix()
	{
		RenderComponent::UpdateWorldMatrix();
		m_world_transpose_matrix = mat4::Transpose(GetWorldMatrix());
		m_world_inverse_matrix = mat4::InverseVec(GetWorldMatrix());
	}

	void MeshComponent::UpdateLevelOfDetail(const Vec3& eye)
	{
		if (IsUseLevelOfDetail())
		{
			float distance = vec3::Length(eye - GetWorldPosition());
			float size = GetMaxExtent() / distance;

			UINT lod = 0;
			if (size > 0.55f)
				lod = 0;
			else if (size <= 0.55f && size > 0.175f)
				lod = 1;
			else if (size <= 0.175f && size > 0.045f)
				lod = 2;
			else if (size <= 0.045f && size > 0.0175f)
				lod = 3;
			else
				lod = 4;
			SetLevelOfDetail(lod);
		}
		else
		{
			SetLevelOfDetail(0);
		}
	}

	void MeshComponent::UpdateLevelOfDetailForShadow(const Vec3& eye)
	{
		if (m_level_of_detail_for_shadow == m_mesh->GetLODCount())
		{
			float distance = vec3::Length(eye - GetWorldPosition());
			float size = GetMaxExtent() / distance;

			UINT lod = 0;
			if (size > 0.55f)
				lod = 0;
			else if (size <= 0.55f && size > 0.175f)
				lod = 1;
			else if (size <= 0.175f && size > 0.045f)
				lod = 2;
			else if (size <= 0.045f && size > 0.0175f)
				lod = 3;
			else
				lod = 4;
			SetLevelOfDetail(lod);

			m_level_of_detail = min(lod, m_mesh->GetLODCount() - 1);
		}
		else
		{
			m_level_of_detail = m_level_of_detail_for_shadow;
		}
	}

	void MeshComponent::ResetLevelOfDetailForShadow()
	{
		m_level_of_detail_for_shadow = m_mesh->GetLODCount();
	}

	void MeshComponent::UpdateOrientedBox()
	{
		if (m_mesh != nullptr)
		{
			m_oriented_box->Transform(m_mesh->GetOrientedBox(), GetWorldMatrix());
			m_max_extent = vec3::Length(m_oriented_box->GetExtents());
		}
	}

	bool MeshComponent::RegisterToRenderSystem()
	{
		if (m_mesh != nullptr)
			return RenderComponent::RegisterToRenderSystem();
		return true;
	}

	void MeshComponent::UnregisterFromRenderSystem()
	{
		if (m_mesh != nullptr)
			RenderComponent::UnregisterFromRenderSystem();
	}

	void MeshComponent::RegisterToVisualOctree()
	{
		if (m_mesh != nullptr)
			RenderComponent::RegisterToVisualOctree();
	}

	void MeshComponent::UnregisterFromVisualOctree()
	{
		if (m_mesh != nullptr)
			RenderComponent::UnregisterFromVisualOctree();
	}

	bool MeshComponent::IsUseLevelOfDetail() const
	{
		return m_mesh->GetLODCount() > 1;
	}

	void MeshComponent::SetLevelOfDetail(UINT lod)
	{
		m_level_of_detail = min(lod, m_mesh->GetLODCount() - 1);
		m_level_of_detail_for_shadow = min(m_level_of_detail_for_shadow, m_level_of_detail);
	}

	void MeshComponent::SetVisiblity(bool value)
	{
		RenderComponent::SetVisiblity(value);
		m_mesh->SetVisible(value);
	}
}