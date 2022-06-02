#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/actor/light.h>

#include "object/level/rendering/basic/normal_map_level.h"
#include "object/levelsetting/rendering/basic/normal_map_level_node_manager.h"

namespace simulation
{
	NormalMapLevelInitNodeManager::NormalMapLevelInitNodeManager()
	{
	}

	NormalMapLevelRuntimeNodeManager::NormalMapLevelRuntimeNodeManager()
	{
		static auto LightTransformFunc = [](const SPtr<NormalMapLevel>& level)
		{
			const auto& directional_light = level->GetDirectionalLight();

			Vec3 rot = quat::QuaternionToEuler(directional_light->GetRotation());
			rot.x = math::ToDegrees(rot.x);
			rot.y = math::ToDegrees(rot.y);
			rot.z = math::ToDegrees(rot.z);
			if (ImGui::DragFloat3("Rotation", (float*)&rot, 0.5f, -180.0f, 180.0f, "%.4f"))
			{
				directional_light->SetRotation(quat::CreateQuaternionFromRollPitchYaw(
					math::ToRadian(rot.x), math::ToRadian(rot.y), math::ToRadian(rot.z)));
			}
		};

		RegisterSettingHeaderNode("Directional light", {
			{ "transform", LightTransformFunc },
			});
	}
}