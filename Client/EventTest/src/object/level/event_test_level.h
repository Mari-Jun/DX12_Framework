#pragma once
#include <client/object/level/core/level.h>
#include <queue>

namespace event_test
{
	using namespace client_fw;

	class EventTestLevel : public Level
	{
	public:
		EventTestLevel();
		virtual ~EventTestLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time);

	public:
		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
		virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const override;

	private:
		Vec3 m_spawn_pos;
		Vec2 m_spawn_ui_pos;

		std::queue<SPtr<class BillboardActor>> m_move_cube_queue;
		std::queue<SPtr<class MaterialBillboardActor>> m_mat_bb_queue;
	};
}



