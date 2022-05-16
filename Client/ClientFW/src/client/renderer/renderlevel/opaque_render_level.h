#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class OpaqueRenderLevel final : public GraphicsRenderLevel
	{
	public:
		OpaqueRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~OpaqueRenderLevel() = default;
	};
}


