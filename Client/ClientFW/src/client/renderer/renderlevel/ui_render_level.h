#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class UIRenderLevel : public GraphicsRenderLevel
	{
	public:
		UIRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~UIRenderLevel() = default;
	};
}


