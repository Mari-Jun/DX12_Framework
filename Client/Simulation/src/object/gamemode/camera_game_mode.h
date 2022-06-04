#pragma once
#include <client/object/level/gamemode/game_mode_base.h>

namespace simulation
{
    using namespace client_fw;

    class CameraGameMode : public GameMode
    {
    public:
        CameraGameMode();
        virtual ~CameraGameMode() = default;
    };
}