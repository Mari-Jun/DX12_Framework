#pragma once
#include <client/object/level/gamemode/game_mode_base.h>

namespace event_test
{
    using namespace client_fw;

    class ThirdGameMode : public GameMode
    {
    public:
        ThirdGameMode();
        virtual ~ThirdGameMode() = default;
    };
}



