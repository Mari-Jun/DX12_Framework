#include "stdafx.h"
#include "client/renderer/renderlevel/ui_render_level.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	UIRenderLevel::UIRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kUI, root_signature, { DXGI_FORMAT_R8G8B8A8_UNORM }, DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
	}
}
