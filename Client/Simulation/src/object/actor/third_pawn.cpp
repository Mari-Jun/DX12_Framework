#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/character_movement_component.h>
#include <client/object/component/util/render_camera_component.h>
#include <client/object/component/render/sphere_component.h>
#include <client/object/component/render/box_component.h>
#include <client/object/actor/player_controller.h>
#include "third_pawn.h"

namespace simulation
{
	ThirdPawn::ThirdPawn()
		: DefaultPawn("third pawn")
	{
		m_movement_component = CreateSPtr<CharacterMovementComponent>();
	}

	bool ThirdPawn::Initialize()
	{
		bool ret = true;

		ret &= Pawn::Initialize();
		ret &= AttachComponent(m_movement_component);
		//Character라면 movement component를 character movement comp로 설정하면 된다. (즉 cast 안해도 된다.)
		std::static_pointer_cast<CharacterMovementComponent>(m_movement_component)->UseOrientRotationToMovement(true);

		auto mesh = CreateSPtr<StaticMeshComponent>();
		ret &= mesh->SetMesh("../Contents/penguin.obj");
		mesh->SetCollisionInfo(true, false, "player", { "sphere" }, true);
		mesh->SetLocalScale(10.0f);
		mesh->SetLocalRotation(quat::CreateQuaternionFromAxis(vec3::AXIS_Y, math::ToRadian(180.0f)));
		ret &= AttachComponent(mesh);

		//auto sphere = CreateSPtr<BoxComponent>(Vec3(100.0f, 100.0f, 100.0f));
		//auto sphere = CreateSPtr<SphereComponent>(100.0f);
		//ret &= AttachComponent(sphere);

		auto camera = CreateSPtr<RenderCameraComponent>();
		const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
		if (player_controller != nullptr)
			player_controller->SetPlayerCamera(camera);
		ret &= AttachComponent(camera);
		camera->UseControllerRotation(true);

	/*	SetUseControllerPitch(true);
		SetUseControllerYaw(true);
		SetUseControllerRoll(true);*/

		return ret;
	}

	void ThirdPawn::Shutdown()
	{
		DefaultPawn::Shutdown();
	}

	void ThirdPawn::Update(float delta_time)
	{
		DefaultPawn::Update(delta_time);
	}
}
