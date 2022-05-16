#include "stdafx.h"
#include "client/renderer/renderlevel/final_view_render_level.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	FinalViewRenderLevel::FinalViewRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kFinalView, root_signature, { DXGI_FORMAT_R8G8B8A8_UNORM }, DXGI_FORMAT_UNKNOWN)
	{
	}
}