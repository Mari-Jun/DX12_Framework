#pragma once
#include <client/object/level/gamemode/game_mode_base.h>

namespace simulation
{
    using namespace client_fw;

    class CameraGameMode : public GameMode
    {
    public:
        CameraGameMode(bool use_controller_event = true);
        virtual ~CameraGameMode() = default;
    };
}