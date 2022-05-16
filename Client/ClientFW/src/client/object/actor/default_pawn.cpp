#include "stdafx.h"
#include "client/object/actor/default_pawn.h"
#include "client/object/component/util/simple_movement_component.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/input/input.h"

namespace client_fw
{
	DefaultPawn::DefaultPawn(const std::string& name)
	{
		m_movement_component = CreateSPtr<SimpleMovementComponent>();
	}

	bool DefaultPawn::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);

		auto mesh = CreateSPtr<StaticMeshComponent>();
		ret &= mesh->SetMesh("../Contents/cube.obj");
		//mesh->GetCollisioner()->SetCollisionInfo("pawn", { "default" });
		ret &= AttachComponent(mesh);

		RegisterAxisEvent("move forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetForward(), axis); return true; });
		RegisterAxisEvent("move right", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetRight(), axis); return true; });
		RegisterAxisEvent("move up", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} },
			[this](float axis)->bool { AddMovementInput(GetUp(), axis); return true; });
		RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kXMove, 1.0f} },
			[this](float axis)->bool { 
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				return AddControllerYawInput(axis * relative_pos.x); 
			});
		RegisterAxisEvent("look up", { AxisEventKeyInfo{eKey::kYMove, 1.0f} },
			[this](float axis)->bool {
				IVec2 relative_pos = Input::GetRelativeMousePosition();
				return AddControllerPitchInput(axis * relative_pos.y);
			});

		SetUseControllerPitch(true);
		SetUseControllerYaw(true);
		SetUseControllerRoll(true);

		return ret;
	}

	void DefaultPawn::Shutdown()
	{
		m_movement_component = nullptr;
		Pawn::Shutdown();
	}

	void DefaultPawn::Update(float delta_time)
	{
		Pawn::Update(delta_time);
	}

	void DefaultPawn::AddMovementInput(const Vec3& direction, float scale)
	{
		m_movement_component->AddInputVector(direction * scale);
	}
}
