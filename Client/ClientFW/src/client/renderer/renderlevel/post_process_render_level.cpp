#include "stdafx.h"
#include "client/renderer/renderlevel/post_process_render_level.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	PostProcessRenderLevel::PostProcessRenderLevel(const SPtr<ComputeSuperRootSignature>& root_signature)
		: ComputeRenderLevel(eRenderLevelType::kPostProcess, root_signature)
	{
	}
}
