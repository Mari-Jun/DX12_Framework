#pragma once
#include <client/object/level/core/level.h>

namespace event_test
{
    using namespace client_fw;

    class PhysicsTestLevel : public Level
    {
    public:
        PhysicsTestLevel();
        virtual ~PhysicsTestLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time);
    }; 
}



