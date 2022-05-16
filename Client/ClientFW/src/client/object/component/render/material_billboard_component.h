#pragma once
#include "client/object/component/render/billboard_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	class Material;

	class MaterialBillboardComponent : public BillboardComponent
	{
	public:
	//private:
		MaterialBillboardComponent(const std::string& name = "material billboard",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kMaskedMaterialNormalMapBillboard));
		virtual ~MaterialBillboardComponent() = default;

	private:
		virtual bool RegisterToRenderSystem() override;

	protected:
		SPtr<Material> m_material;

	public:
		const SPtr<Material>& GetMaterial() const { return m_material; }

		//runtime에 Material을 바꾸게 되면 제대로 렌더링이 안될 가능성이 높습니다.
		//if .mtl file (parent releative path + mtl's newmtl name)
		void SetMaterial(const std::string& mtl_path);

		//if .mtl file (path = releative path, mtl_name = mtl's newmtl name)
		void SetMaterial(const std::string& path, const std::string& mtl_name);
		void SetMaterial(const SPtr<Material>& material) { m_material = material; }
	};

}

