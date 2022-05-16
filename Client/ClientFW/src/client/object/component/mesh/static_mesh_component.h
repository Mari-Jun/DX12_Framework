#pragma once
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	class StaticMesh;

	class StaticMeshComponent : public MeshComponent
	{
	public:
		StaticMeshComponent(const std::string& name = "static mesh component",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kOpaqueMaterialMesh));
		virtual ~StaticMeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	private:
		virtual bool RegisterToRenderSystem() override;

	public:
		SPtr<StaticMesh> GetStaticMesh() const;

		//runtime�� Mesh�� �ٲٰ� �Ǹ� ����� �������� �ȵ� ���ɼ��� �����ϴ�.
		virtual bool SetMesh(const std::string& file_path);

	protected:
		SPtr<StaticMeshComponent> SharedFromThis();
	};

}

