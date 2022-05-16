#include "stdafx.h"
#include "client/renderer/core/mesh_visualizer.h"
#include "client/util/octree/octree.h"
#include "client/util/octree/octree_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/util/core/camera_component.h"
#include "client/object/component/core/render_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	constexpr static auto render_execute_function =
		[](const SPtr<RenderComponent>& render_comp, const Vec3& eye)
	{
		render_comp->SetVisiblity(true);
		render_comp->UpdateLevelOfDetail(eye);
	};

	constexpr static auto shadow_execute_function =
		[](const SPtr<RenderComponent>& render_comp, const Vec3& eye)
	{
		render_comp->SetVisiblity(true);
		render_comp->UpdateLevelOfDetailForShadow(eye);
	};

	constexpr static auto trigger_function =
		[](const SPtr<RenderComponent>& render_cmp)
	{
		return (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
	};

	constexpr static auto shadow_trigger_function =
		[](const SPtr<RenderComponent>& render_cmp)
	{
		//Mesh를 제외하면 그림자를 생성하지 않는다.
		return
			(render_cmp->GetRenderType() == eRenderType::kMesh)
			&& (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
	};

	constexpr static auto add_destructible_shadow_trigger_function =
		[](const SPtr<RenderComponent>& render_cmp)
	{
		return (render_cmp->GetRenderType() == eRenderType::kMesh) &&
			(render_cmp->GetOwner().lock()->GetMobilityState() == eMobilityState::kDestructible);
	};

	constexpr static auto static_shadow_trigger_function =
		[](const SPtr<RenderComponent>& render_cmp)
	{
		//Mesh를 제외하면 그림자를 생성하지 않는다.
		return
			(render_cmp->GetRenderType() == eRenderType::kMesh)
			&& (render_cmp->IsVisible() == false) &&
			(render_cmp->GetOwner().lock()->GetMobilityState() == eMobilityState::kStatic);
	};

	void MeshVisualizer::UpdateVisibilityFromMovableRenderCamera(const SPtr<CameraComponent>& camera)
	{
		UpdateVisibilityForRender(camera->GetBoudingFrustum(), camera->GetCameraPosition(),
			trigger_function, render_execute_function);
	}

	void MeshVisualizer::UpdateVisibilityFromMovableShadowCamera(const SPtr<CameraComponent>& camera)
	{
		UpdateVisibilityForShadow(camera->GetBoudingFrustum(), camera->GetCameraPosition(), 
			shadow_trigger_function, shadow_execute_function);
	}

	void MeshVisualizer::UpdateVisibilityFromMovableShadowSphere(const SPtr<CameraComponent>& camera, const Vec3& eye, float radius)
	{
		BSphere bounding_sphere = BSphere(eye, radius);

		UpdateVisibilityForShadow(bounding_sphere, eye, shadow_trigger_function, shadow_execute_function);
	}

	void MeshVisualizer::UpdateVisibilityFromStaticShadowCamera(const SPtr<CameraComponent>& camera, bool update_static_render_comp)
	{
		if (update_static_render_comp)
		{
			UpdateVisibilityForStatic(camera->GetBoudingFrustum(), camera->GetCameraPosition(),
				static_shadow_trigger_function, render_execute_function);
			AddDestructibleFromVisualTreeNodeToCamera(camera, camera->GetBoudingFrustum(), camera->GetCameraPosition(), 
				add_destructible_shadow_trigger_function);
		}
		else
		{
			AddRegisteredDestructibleToCamera(camera, camera->GetBoudingFrustum(), add_destructible_shadow_trigger_function);
			camera->UpdateDestructibleRenderComponentVisibility(trigger_function);
			UpdateVisibilityMovableForShadow(camera->GetBoudingFrustum(), camera->GetCameraPosition(),
				trigger_function, shadow_execute_function);
		}
	}

	void MeshVisualizer::UpdateVisibilityFromStaticShadowSphere(const SPtr<CameraComponent>& camera,
		const Vec3& eye, float radius, bool update_static_render_comp)
	{
		BSphere bounding_sphere = BSphere(eye, radius);

		if (update_static_render_comp)
		{
			UpdateVisibilityForStatic(bounding_sphere, eye, static_shadow_trigger_function, render_execute_function);
			AddDestructibleFromVisualTreeNodeToCamera(camera, bounding_sphere, eye,
				add_destructible_shadow_trigger_function);
		}
		else
		{
			AddRegisteredDestructibleToCamera(camera, bounding_sphere, add_destructible_shadow_trigger_function);
			camera->UpdateDestructibleRenderComponentVisibility(trigger_function);
			UpdateVisibilityMovableForShadow(bounding_sphere, eye, trigger_function, shadow_execute_function);
		}
	}
}
