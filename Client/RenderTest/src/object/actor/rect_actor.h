#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace render_test
{
    using namespace client_fw;

    class RectActor : public StaticMeshActor
    {
	public:
		RectActor();
		virtual ~RectActor() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
    };


}

