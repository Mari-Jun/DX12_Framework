#pragma once
#include <client/object/level/core/level.h>

namespace event_test
{
    using namespace client_fw;

    class LightTestLevel : public Level
    {
    public:
        LightTestLevel();
        virtual ~LightTestLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    public:
        virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const override;
    };
}



