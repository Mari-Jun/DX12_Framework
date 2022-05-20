#pragma once
#include <client/object/layer/core/layer.h>

namespace simulation
{
    using namespace client_fw;

    class SimulationLevel;

    class LevelSettingLayer final : public Layer
    {
    public:
        LevelSettingLayer();
        virtual ~LevelSettingLayer() = default;

        virtual void Update(float delta_time) override;

    private:
        SPtr<SimulationLevel> m_cur_level;

    public:
        void SetCurrentSimulationLevel(const SPtr<SimulationLevel>& level) { m_cur_level = level; }
    };

}



