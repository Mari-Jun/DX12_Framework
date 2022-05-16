#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class FinalViewRenderLevel : public GraphicsRenderLevel
	{
	public:
		FinalViewRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature);
		virtual ~FinalViewRenderLevel() = default;
	};


}

