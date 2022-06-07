#pragma once
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	class PostProcessRenderLevel :public ComputeRenderLevel
	{
	public:
		PostProcessRenderLevel(const SPtr<ComputeSuperRootSignature>& root_signature);
		virtual ~PostProcessRenderLevel() = default;
	};

}

