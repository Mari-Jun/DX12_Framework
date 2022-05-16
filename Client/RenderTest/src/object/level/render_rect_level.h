#pragma once
#include "client/object/level/core/level.h"
#include <queue>

namespace render_test
{
    using namespace client_fw;

    class RenderRectLevel final : public Level
    {
    public:
        RenderRectLevel(const std::string& name = "render_rect_level");
        virtual ~RenderRectLevel() = default;

        virtual bool Initialize() override;
        virtual void Shutdown() override;

        virtual void Update(float delta_time) override;

    private:
        std::queue<SPtr<class Actor>> m_actors;
    };
}



