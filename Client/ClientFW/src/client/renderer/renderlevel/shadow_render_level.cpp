#include "stdafx.h"
#include "client/renderer/renderlevel/shadow_render_level.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	ShadowRenderLevel::ShadowRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kShadow, root_signature,
			{}, DXGI_FORMAT_D32_FLOAT)
	{
	}

	ShadowCubeRenderLevel::ShadowCubeRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kShadowCube, root_signature,
			{}, DXGI_FORMAT_D32_FLOAT)
	{
	}

	ShadowCascadeRenderLevel::ShadowCascadeRenderLevel(const SPtr<GraphicsSuperRootSignature>& root_signature)
		: GraphicsRenderLevel(eRenderLevelType::kShadowCascade, root_signature,
			{}, DXGI_FORMAT_D32_FLOAT)
	{
	}
}
