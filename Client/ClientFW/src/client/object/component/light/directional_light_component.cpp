#include "stdafx.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	DirectionalLightComponent::DirectionalLightComponent()
		: LightComponent(eLightType::kDirectional, "directional light component",
			Render::ConvertShaderType(eShaderType::kDeferred))
	{
		m_shadow_texture_size = 2048;
	}

	bool DirectionalLightComponent::Initialize()
	{
		bool ret = LightComponent::Initialize();
		m_owner.lock()->UseUpdate();
		return ret;
	}

	void DirectionalLightComponent::RegisterCascadeAndRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp,
		const SPtr<ShadowCascadeCameraComponent>& cascade_camera_comp)
	{
		CascadeShadowForRenderCamera cascade_shadow{ render_camera_comp, cascade_camera_comp };

		auto iter = find_if(m_cascade_shadows_camera.begin(), m_cascade_shadows_camera.end(),
			[cascade_shadow](const CascadeShadowForRenderCamera& cs_camera)
			{ return cs_camera.render_camera.lock() == cascade_shadow.render_camera.lock(); });

		if (iter == m_cascade_shadows_camera.end())
		{
			cascade_camera_comp->SetRenderCamera(render_camera_comp);
			cascade_camera_comp->SetViewport(Viewport{ 0, 0, m_shadow_texture_size, m_shadow_texture_size });
			m_owner.lock()->AttachComponent(cascade_camera_comp);
			m_cascade_shadows_camera.emplace_back(std::move(cascade_shadow));
		}
	}

	void DirectionalLightComponent::UnregisterRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp)
	{
		auto iter = find_if(m_cascade_shadows_camera.begin(), m_cascade_shadows_camera.end(),
			[render_camera_comp](const CascadeShadowForRenderCamera& cs_camera)
			{ return cs_camera.render_camera.lock() == render_camera_comp.lock(); });

		if (iter != m_cascade_shadows_camera.end())
			m_cascade_shadows_camera.erase(iter);
	}

	void DirectionalLightComponent::UpdateShadowTextureSize()
	{
		INT extent = std::clamp(m_shadow_texture_size, 0, s_directional_light_max_texture_size);
	}
}
