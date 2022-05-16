#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace event_test
{
	using namespace client_fw;

	class MoveCube final : public StaticMeshActor
	{
	public:
		MoveCube();
		virtual ~MoveCube() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		Vec3 m_first_pos;
		float m_speed = 2000.0f;
	};

}


