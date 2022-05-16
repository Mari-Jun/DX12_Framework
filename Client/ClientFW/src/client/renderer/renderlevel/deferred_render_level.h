#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
    class DeferredRenderLevel : public GraphicsRenderLevel
    {
    public:
        DeferredRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
        virtual ~DeferredRenderLevel() = default;
    };
}



