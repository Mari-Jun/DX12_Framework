#include <include/client_core.h>
#include <include/dx12_shader_fw.h>

#include <client/object/actor/static_mesh_actor.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/asset/mesh/mesh.h>
#include <client/util/file_dialog.h>

#include "object/level/rendering/basic/import_obj_mesh_level.h"
#include "object/levelsetting/rendering/basic/import_obj_mesh_level_node_manager.h"

namespace simulation
{
	ImportObjMeshLevelInitNodeManager::ImportObjMeshLevelInitNodeManager()
	{
	}

	ImportObjMeshLevelRuntimeNodeManager::ImportObjMeshLevelRuntimeNodeManager()
	{
		static auto TransformFunc = [](const SPtr<ImportObjMeshLevel>& level)
		{
			const auto& static_mesh_actor = level->GetStaticMeshActor();

			Vec3 pos = static_mesh_actor->GetPosition();
			if (ImGui::DragFloat3("Position", (float*)&pos, 0.5f, -FLT_MAX, FLT_MAX, "%.4f"))
			{
				static_mesh_actor->SetPosition(pos);
			}

			Vec3 rot = quat::QuaternionToEuler(static_mesh_actor->GetRotation());
			rot.x = math::ToDegrees(rot.x);
			rot.y = math::ToDegrees(rot.y);
			rot.z = math::ToDegrees(rot.z);
			if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
			{
				static_mesh_actor->SetRotation(quat::CreateQuaternionFromRollPitchYaw(
					math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
			}

			Vec3 scale = static_mesh_actor->GetScale();
			if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f, 0.0f, FLT_MAX, "%.2f"))
			{
				static_mesh_actor->SetScale(scale);
			}
		};

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
			{"set path", StaticMeshPathFunc},
			{"transform", TransformFunc},
			});
	}
}
