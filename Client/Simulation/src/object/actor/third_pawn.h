#pragma once
#include <client/object/actor/default_pawn.h>

namespace simulation
{
    using namespace client_fw;

    class ThirdPawn : public DefaultPawn
    {
    public:
        ThirdPawn();
        virtual ~ThirdPawn() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;
    };
}