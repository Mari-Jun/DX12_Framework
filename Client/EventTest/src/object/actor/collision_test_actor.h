#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace event_test
{
    using namespace client_fw;

    class CollisionTestActor : public StaticMeshActor
    {
    public:
        CollisionTestActor();
        virtual ~CollisionTestActor() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    private:
        bool m_is_box;
        bool m_is_col = true;
        float m_time = 0.0f;
    };

}


