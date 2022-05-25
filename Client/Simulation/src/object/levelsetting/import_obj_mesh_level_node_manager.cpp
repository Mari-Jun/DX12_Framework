#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/object/actor/static_mesh_actor.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/asset/mesh/mesh.h>
#include <client/util/file_dialog.h>

#include "object/level/import_obj_mesh_level.h"
#include "object/levelsetting/import_obj_mesh_level_node_manager.h"

namespace simulation
{
	ImportObjMeshLevelInitNodeManager::ImportObjMeshLevelInitNodeManager()
	{
	}

	ImportObjMeshLevelRuntimeNodeManager::ImportObjMeshLevelRuntimeNodeManager()
	{
		static auto StaticMeshPathFunc = [](const SPtr<ImportObjMeshLevel>& level)
		{
			std::string path;
			if (level->GetStaticMeshActor() != nullptr)
			{
				const auto& static_mesh_comp = level->GetStaticMeshActor()->GetStaticMeshComponent();
				if (static_mesh_comp != nullptr && static_mesh_comp->GetMesh() != nullptr)
				{
					path = static_mesh_comp->GetMesh()->GetPath();
				}
			}

			ImGui::BulletText(path.c_str());

			if (ImGui::Button("Open file dialog"))
			{
				path = FileDialog::OpenFile(L"obj file\0*.obj");
				if (path.empty() == false)
				{

					level->SetStaticMeshPath(path);
				}
			}
		};

		RegisterSettingHeaderNode("Static mesh actor", {
			{"set path", StaticMeshPathFunc}
			});
	}
}
