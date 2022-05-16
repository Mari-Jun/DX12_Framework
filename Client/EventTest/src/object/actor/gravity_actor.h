#pragma once
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
    class ProjectileMovementComponent;
}

namespace event_test
{
    using namespace client_fw;

    class GravityActor : public StaticMeshActor
    {
    public:
        GravityActor(const std::string& path);
        virtual ~GravityActor() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    private:
        SPtr<ProjectileMovementComponent> m_projectile_movement_component;
    };
}


