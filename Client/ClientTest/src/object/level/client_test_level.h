#pragma once
#include "client/object/level/core/level.h"
#include <queue>

namespace client_test
{
    using namespace client_fw;

    class ClientTestLevel final: public Level
    {
    public:
        ClientTestLevel(const std::string& name = "client_test_level");
        virtual ~ClientTestLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    private:
        std::queue<SPtr<class Actor>> m_actors;
        int m_actor_count = 0;
    };
}


