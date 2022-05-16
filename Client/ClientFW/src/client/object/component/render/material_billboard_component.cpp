#include "stdafx.h"
#include "client/object/component/render/material_billboard_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/material/material.h"

namespace client_fw
{
	MaterialBillboardComponent::MaterialBillboardComponent(const std::string& name,
		const std::string& draw_shader_name)
		: BillboardComponent(name, eBillboardType::kMaterial, draw_shader_name)
	{
	}

	bool MaterialBillboardComponent::RegisterToRenderSystem()
	{
		// 기본으로 지원하는 Shader를 사용하는 경우
		// 현재 장착중인 material에 가장 알맞은 shader를 선택해준다.
		if (m_material != nullptr)
		{
			if (m_draw_shader_name == Render::ConvertShaderType(eShaderType::kMaskedMaterialBaseColorBillboard) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kMaskedMaterialTextureBillboard) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kMaskedMaterialNormalMapBillboard))
			{
				if (m_material->GetDiffuseTexture() != nullptr)
				{
					if (m_material->GetNormalTexture() != nullptr)
						SetDrawShaderName(Render::ConvertShaderType(eShaderType::kMaskedMaterialNormalMapBillboard));
					else
						SetDrawShaderName(Render::ConvertShaderType(eShaderType::kMaskedMaterialTextureBillboard));
				}
				else
					SetDrawShaderName(Render::ConvertShaderType(eShaderType::kMaskedMaterialBaseColorBillboard));
			}
			else if (m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueMaterialBaseColorBillboard) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueMaterialTextureBillboard) ||
				m_draw_shader_name == Render::ConvertShaderType(eShaderType::kOpaqueMaterialNormalMapBillboard))
			{
				if (m_material->GetDiffuseTexture() != nullptr)
				{
					if (m_material->GetNormalTexture() != nullptr)
						SetDrawShaderName(Render::ConvertShaderType(eShaderType::kOpaqueMaterialNormalMapBillboard));
					else
						SetDrawShaderName(Render::ConvertShaderType(eShaderType::kOpaqueMaterialTextureBillboard));
				}
				else
					SetDrawShaderName(Render::ConvertShaderType(eShaderType::kOpaqueMaterialBaseColorBillboard));
			}

			return RenderComponent::RegisterToRenderSystem();
		}
		else
		{
			return false;
		}

	}

	void MaterialBillboardComponent::SetMaterial(const std::string& mtl_path)
	{
		const auto& material = AssetStore::LoadMaterial(mtl_path);
		if (material != nullptr)
			SetMaterial(material);
		else
			LOG_ERROR("Could not find material : {0}", mtl_path);
	}

	void MaterialBillboardComponent::SetMaterial(const std::string& path, const std::string& mtl_name)
	{
		const auto& material = AssetStore::LoadMaterial(path, mtl_name);
		if (material != nullptr)
			SetMaterial(material);
		else
			LOG_ERROR("Could not find material : {0} - {1}", path, mtl_name);
	}
}