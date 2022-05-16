#pragma once
#include "client/object/actor/pawn.h"

namespace client_fw
{
	class StaticMeshComponent;
	class PawnMovementComponent;

	class DefaultPawn : public Pawn
	{
	public:
		DefaultPawn(const std::string& name = "default pawn");
		virtual ~DefaultPawn() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		virtual void AddMovementInput(const Vec3& direction, float scale) override;

	protected:
		SPtr<PawnMovementComponent> m_movement_component;

	protected:
		using Pawn::SetUseControllerPitch;
		using Pawn::SetUseControllerYaw;
		using Pawn::SetUseControllerRoll;
	};
}



