#include "stdafx.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/material/material.h"
#include "client/renderer/core/render.h"
#include "client/physics/collision/collisioner/static_mesh_collisioner.h"

namespace client_fw
{
	StaticMeshComponent::StaticMeshComponent(const std::string& name, const std::string& draw_shader_name)
		: MeshComponent(name, draw_shader_name)
	{
		if (m_collisioner == nullptr)
			m_collisioner = CreateUPtr<StaticMeshCollisioner>();
	}

	bool StaticMeshComponent::Initialize()
	{
		return MeshComponent::Initialize();
	}

	void StaticMeshComponent::Shutdown()
	{
		MeshComponent::Shutdown();
	}

	bool StaticMeshComponent::RegisterToRenderSystem()
	{
		if (m_mesh != nullptr)
		{
			if (m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueMaterialMesh) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueTextureMesh) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueNormalMapMesh))
			{
				m_draw_shader_name = Render::ConvertShaderType(eShaderType::kOpaqueMaterialMesh);

				bool is_convert_texture = false;
				for (UINT lod = 0; lod < m_mesh->GetLODCount(); ++lod)
				{
					const auto& materials = m_mesh->GetMaterials(lod);
					for (const auto& material : materials)
					{
						//Diffuse Texture를 사용하는 경우는 OpaqueTextureMesh Shader를 사용해서 그린다.
						if (is_convert_texture == false && material->GetDiffuseTexture() != nullptr)
						{
							m_draw_shader_name = Render::ConvertShaderType(eShaderType::kOpaqueTextureMesh);
							is_convert_texture = true;
						}

						//Diffuse Texture뿐만 아니라 Normal Texture까지 사용하는 경우는 OpaqueNormalMapMesh Shader를 사용해서 그린다.
						if (is_convert_texture == true && material->GetNormalTexture() != nullptr)
							m_draw_shader_name = Render::ConvertShaderType(eShaderType::kOpaqueNormalMapMesh);
					}
				}
			}
		}

		return MeshComponent::RegisterToRenderSystem();
	}

	SPtr<StaticMesh> StaticMeshComponent::GetStaticMesh() const
	{
		return std::static_pointer_cast<StaticMesh>(m_mesh);
	}

	bool StaticMeshComponent::SetMesh(const std::string& file_path)
	{
		if (file_path.empty() == false)
		{
			if (m_mesh != nullptr)
			{
				UnregisterFromVisualOctree();
				UnregisterFromRenderSystem();
			}

			m_mesh =  std::dynamic_pointer_cast<StaticMesh>(AssetStore::LoadMesh(file_path));
			if (m_mesh == nullptr)
			{
				LOG_ERROR("Could not cast Mesh[{0}] to StaticMesh", file_path);
			}
			else
			{
				if (m_owner.lock() != nullptr)
				{
					RegisterToRenderSystem();
					RegisterToVisualOctree();
					m_update_local_matrix = true;
				}

			}
		}
		else
		{
			LOG_WARN("file path is empty");
		}
		return true;
	}

	SPtr<StaticMeshComponent> StaticMeshComponent::SharedFromThis()
	{
		return std::static_pointer_cast<StaticMeshComponent>(shared_from_this());
	}
}
