#pragma once
#include "client/object/layer/core/layer.h"

namespace simulation
{
    using namespace client_fw;

    class ViewportLayer : public Layer
    {
    public:
        ViewportLayer(const std::string& name = "viewport layer");
        virtual ~ViewportLayer() = default;

        virtual void Update(float delta_time) override;

    private:
        static IVec2 s_viewport_pos;
        static IVec2 s_viewport_size;
    };
}

