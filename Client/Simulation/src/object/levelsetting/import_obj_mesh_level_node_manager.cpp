#include <include/client_core.h>
#include <client/object/actor/static_mesh_actor.h>
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
			if (ImGui::Button("Open file dialog"))
			{
				std::string path = FileDialog::OpenFile(L"obj file\0*.obj");
				if (path.empty() == false)
					level->SetStaticMeshPath(path);
			}
		};

		RegisterSettingHeaderNode("Static mesh actor", {
			{"set path", StaticMeshPathFunc}
			});
	}
}
