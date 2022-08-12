#include <include/client_core.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/actor/pawn.h>
#include <client/input/input.h>
#include "third_player_controller.h"

namespace simulation
{
	ThirdPlayerController::ThirdPlayerController()
		: PlayerController("third player controller")
	{
	}

	bool ThirdPlayerController::Initialize()
	{
		bool ret = PlayerController::Initialize();

		const auto& camera = GetPlayerCamera();
		camera->SetLocalPosition(Vec3(0.0f, 50.0f, -500.0f));

		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetForward(), axis); return true; });
		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetRight(), axis); return true; });
		RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
			[this](float axis)->bool { m_controlled_pawn->AddMovementInput(GetUp(), axis); return true; });
		RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
			[this](float axis)->bool {
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				AddYawInput(axis * relative_pos.x);
				return true;
			});
		RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
			[this](float axis)->bool {
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				AddPitchInput(axis * relative_pos.y);
				return true;
			});

		return ret;
	}

	void ThirdPlayerController::Shutdown()
	{
		PlayerController::Shutdown();
	}
}
