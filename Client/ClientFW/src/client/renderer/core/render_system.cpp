#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"

#include "client/renderer/renderlevel/opaque_render_level.h"
#include "client/renderer/renderlevel/shadow_render_level.h"
#include "client/renderer/renderlevel/deferred_render_level.h"
#include "client/renderer/renderlevel/final_view_render_level.h"
#include "client/renderer/renderlevel/ui_render_level.h"

#include "client/renderer/shader/opaque_mesh_shader.h"	
#include "client/renderer/shader/box_shape_shader.h"
#include "client/renderer/shader/deferred_shader.h"
#include "client/renderer/shader/main_camera_ui_shader.h"
#include "client/renderer/shader/ui_shader.h"
#include "client/renderer/shader/texture_billboard_shader.h"
#include "client/renderer/shader/material_billboard_shader.h"
#include "client/renderer/shader/widget_shader.h"
#include "client/renderer/shader/sky_shader.h"
#include "client/renderer/shader/light_shader.h"

#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/core/render_camera_manager.h"
#include "client/renderer/core/shadow_camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/imgui/imgui_system.h"

#include "client/object/component/core/render_component.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/render/shape_component.h"
#include "client/object/component/render/billboard_component.h"
#include "client/object/component/render/widget_component.h"
#include "client/object/component/util/core/camera_component.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/component/light/core/light_component.h"
#include "client/object/component/light/core/local_light_component.h"
#include "client/object/component/sky/sky_component.h"

#include <stdio.h>
#include <time.h>
//#define __USE_RENDER_UPDATE_CPU_TIME__
//#define __USE_RENDER_DRAW_CPU_TIME__

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem(const WPtr<Window>& window)
		: m_window(window)
	{
		Render::s_render_system = this;
		m_graphics_super_root_signature = CreateSPtr<GraphicsSuperRootSignature>();

		m_render_asset_manager = CreateUPtr<RenderResourceManager>();
		m_render_camera_manager = CreateUPtr<RenderCameraManager>();
		m_shadow_camera_manager = CreateUPtr<ShadowCameraManager>();
		m_light_manager = CreateUPtr<LightManager>();
		m_imgui_system = CreateUPtr<ImGuiSystem>();
	}

	RenderSystem::~RenderSystem()
	{
	}

	bool RenderSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_device = device;

		bool ret = m_graphics_super_root_signature->Initialize(device, command_list);

		ret &= RegisterGraphicsRenderLevel<OpaqueRenderLevel>(eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsRenderLevel<ShadowRenderLevel>(eRenderLevelType::kShadow);
		ret &= RegisterGraphicsRenderLevel<ShadowCubeRenderLevel>(eRenderLevelType::kShadowCube);
		ret &= RegisterGraphicsRenderLevel<ShadowCascadeRenderLevel>(eRenderLevelType::kShadowCascade);
		ret &= RegisterGraphicsRenderLevel<DeferredRenderLevel>(eRenderLevelType::kDeferred);
		ret &= RegisterGraphicsRenderLevel<FinalViewRenderLevel>(eRenderLevelType::kFinalView);
		ret &= RegisterGraphicsRenderLevel<UIRenderLevel>(eRenderLevelType::kUI);

		ret &= RegisterGraphicsShader<OpaqueMaterialMeshShader>(Render::ConvertShaderType(eShaderType::kOpaqueMaterialMesh),
			{ eRenderLevelType::kOpaque, eRenderLevelType::kShadow, eRenderLevelType::kShadowCube, eRenderLevelType::kShadowCascade });
		ret &= RegisterGraphicsShader<OpaqueTextureMeshShader>(Render::ConvertShaderType(eShaderType::kOpaqueTextureMesh),
			{ eRenderLevelType::kOpaque, eRenderLevelType::kShadow, eRenderLevelType::kShadowCube, eRenderLevelType::kShadowCascade });
		ret &= RegisterGraphicsShader<OpaqueNormalMapMeshShader>(Render::ConvertShaderType(eShaderType::kOpaqueNormalMapMesh),
			{ eRenderLevelType::kOpaque, eRenderLevelType::kShadow, eRenderLevelType::kShadowCube, eRenderLevelType::kShadowCascade });
		//ret &= RegisterGraphicsShader<BoxShapeShader>("shape box", { eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<TextureBillboardShader>(Render::ConvertShaderType(eShaderType::kTextureBillboard),
			{eRenderLevelType::kOpaque});
		ret &= RegisterGraphicsShader<OpaqueMaterialBaseColorBillboardShader>
			(Render::ConvertShaderType(eShaderType::kOpaqueMaterialBaseColorBillboard), 
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<MaskedMaterialBaseColorBillboardShader>
			(Render::ConvertShaderType(eShaderType::kMaskedMaterialBaseColorBillboard), 
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<OpaqueMaterialTextureBillboardShader>
			(Render::ConvertShaderType(eShaderType::kOpaqueMaterialTextureBillboard),
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<MaskedMaterialTextureBillboardShader>
			(Render::ConvertShaderType(eShaderType::kMaskedMaterialTextureBillboard),
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<OpaqueMaterialNormalMapBillboardShader>
			(Render::ConvertShaderType(eShaderType::kOpaqueMaterialNormalMapBillboard),
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<MaskedMaterialNormalMapBillboardShader>
			(Render::ConvertShaderType(eShaderType::kMaskedMaterialNormalMapBillboard),
				{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<OpaqueWidgetShader>(Render::ConvertShaderType(eShaderType::kOpaqueWidget),
			{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<MaskedWidgetShader>(Render::ConvertShaderType(eShaderType::kMaskedWidget),
			{ eRenderLevelType::kOpaque });
		ret &= RegisterGraphicsShader<SkyShader>(Render::ConvertShaderType(eShaderType::kSky), { eRenderLevelType::kOpaque });

		ret &= RegisterGraphicsShader<DeferredShader>(Render::ConvertShaderType(eShaderType::kDeferred),
			{ eRenderLevelType::kDeferred });
		ret &= RegisterGraphicsShader<PointLightShader>(Render::ConvertShaderType(eShaderType::kPointLight),
			{ eRenderLevelType::kDeferred });
		ret &= RegisterGraphicsShader<SpotLightShader>(Render::ConvertShaderType(eShaderType::kSpotLight),
			{ eRenderLevelType::kDeferred });

		ret &= RegisterGraphicsShader<MainCameraUIShader>("main camera ui", { eRenderLevelType::kFinalView });
		ret &= RegisterGraphicsShader<UIShader>("ui", { eRenderLevelType::kUI });

		ret &= m_imgui_system->Initialize(device, m_window.lock()->hWnd);
		ret &= m_render_asset_manager->Initialize(device);

		return ret;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_graphics_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_graphics_shaders)
			shader->Shutdown();
		m_graphics_super_root_signature->Shutdown();
		m_imgui_system->Shutdown();
		m_render_asset_manager->Shutdown();
		m_light_manager->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device)
	{
#ifdef __USE_RENDER_UPDATE_CPU_TIME__
		clock_t l_start, l_end;
		l_start = clock();
#endif
		m_render_camera_manager->Update(device,
			[this](ID3D12Device* device) {
				m_graphics_render_levels.at(eRenderLevelType::kOpaque)->Update(device);
				m_graphics_render_levels.at(eRenderLevelType::kDeferred)->Update(device);
			});

		m_light_manager->Update(device);

		m_shadow_camera_manager->Update(device,
			[this](ID3D12Device* device) {
				m_graphics_render_levels.at(eRenderLevelType::kShadow)->Update(device);
			},
			[this](ID3D12Device* device) {
				m_graphics_render_levels.at(eRenderLevelType::kShadowCube)->Update(device);
			},
				[this](ID3D12Device* device) {
				m_graphics_render_levels.at(eRenderLevelType::kShadowCascade)->Update(device);
			});

		if (m_render_camera_manager->GetMainCamera() != nullptr)
		{
			m_graphics_render_levels.at(eRenderLevelType::kFinalView)->Update(device);
		}
		m_graphics_render_levels.at(eRenderLevelType::kUI)->Update(device);


		for (const auto& [level_type, render_level] : m_graphics_render_levels)
			render_level->UpdateFrameResource(device);

		m_imgui_system->Update(device);

#ifdef __USE_RENDER_UPDATE_CPU_TIME__
		l_end = clock();
#ifdef _DEBUG
		LOG_INFO("Render Update Time : {0}", float(l_end - l_start));
#else
		std::cout << "Render Update Time " << float(l_end - l_start) << std::endl;
#endif
#endif
	}


	void RenderSystem::PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) const
	{
		m_render_asset_manager->PreDraw(device, command_list);
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		//지금 이 부분은 Shadow나 Compute가 생길 경우 많이 바뀔 부분이다.
		//일단은 Opaque정도만 신경써서 코딩을 하였다.
		//지금으로서는 능력 부족으로 정확한 예측은 커녕 50%의 예측도 불가능하다.

#ifdef __USE_RENDER_DRAW_CPU_TIME__
		clock_t l_start, l_end;
		l_start = clock();
#endif

		m_graphics_super_root_signature->Draw(command_list);
		m_render_asset_manager->Draw(command_list);
		m_light_manager->Draw(command_list);

		if (m_render_camera_manager->GetMainCamera() != nullptr)
		{
			m_shadow_camera_manager->Draw(command_list,
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kShadow)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kShadowCube)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kShadowCascade)->Draw(command_list);
				});

			m_render_camera_manager->Draw(command_list,
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kOpaque)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kDeferred)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
				});
		}

#ifdef __USE_RENDER_DRAW_CPU_TIME__
		l_end = clock();
#ifdef _DEBUG
		LOG_TRACE("Render Draw Time : {0}", float(l_end - l_start));
#else
		std::cout << "Render Draw Time " << float(l_end - l_start) << std::endl;
#endif
#endif
	}

	void RenderSystem::DrawMainCameraView(ID3D12GraphicsCommandList* command_list) const
	{
		if (m_render_camera_manager->GetMainCamera() != nullptr)
		{
			m_render_camera_manager->DrawMainCameraForUI(command_list);
			m_graphics_render_levels.at(eRenderLevelType::kFinalView)->Draw(command_list);
		}
	}

	void RenderSystem::DrawUI(ID3D12GraphicsCommandList* command_list) const
	{
		m_graphics_render_levels.at(eRenderLevelType::kUI)->Draw(command_list);
		m_imgui_system->PreDraw(command_list);
		m_imgui_system->Draw(command_list);
	}

	void RenderSystem::UpdateViewport()
	{
		const auto& window = m_window.lock();
		m_render_camera_manager->UpdateMainCameraViewport(window->width, window->height);
	}

	void RenderSystem::UnregisterGraphicsShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types)
	{
		for (eRenderLevelType level_type : level_types)
		{
			if (m_added_shaders.find(shader_name) != m_added_shaders.cend() &&
				m_graphics_render_levels.find(level_type) != m_graphics_render_levels.cend())
			{
				auto& shader = m_graphics_shaders[shader_name];
				m_added_shaders.erase(shader_name);
				m_graphics_render_levels[level_type]->UnregisterGraphicsShader(shader);
				shader->Shutdown();
				m_graphics_shaders.erase(shader_name);
			}
		}
	}

	bool RenderSystem::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		switch (render_comp->GetRenderType())
		{
		case eRenderType::kMesh:
		{
			const auto& mesh_comp = std::static_pointer_cast<MeshComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterMeshComponent(m_device, mesh_comp);
		}
		case eRenderType::kShape:
		{
			const auto& shape_comp = std::static_pointer_cast<ShapeComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterShapeComponent(m_device, shape_comp);
		}
		case eRenderType::kBillboard:
		{
			const auto& billboard_comp = std::static_pointer_cast<BillboardComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterBillboardComponent(m_device, billboard_comp);
		}
		case eRenderType::kWidget:
		{
			const auto& widget_comp = std::static_pointer_cast<WidgetComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterWidgetComponent(m_device, widget_comp);
		}
		case eRenderType::kLight:
		{
			const auto& light_comp = std::static_pointer_cast<LightComponent>(render_comp);
			bool ret = m_light_manager->RegisterLightComponent(light_comp);

			const auto& local_light_comp = std::dynamic_pointer_cast<LocalLightComponent>(render_comp);
			if (local_light_comp != nullptr)
				ret &= m_graphics_shaders.at(shader_name)->RegisterLocalLightComponent(m_device, local_light_comp);
			return ret;
		}
		default:
			break;
		}

		return false;
	}

	void RenderSystem::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return;
		}

		switch (render_comp->GetRenderType())
		{
		case eRenderType::kMesh:
		{
			const auto& mesh_comp = std::static_pointer_cast<MeshComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterMeshComponent(mesh_comp);
			break;
		}
		case eRenderType::kShape:
		{
			const auto shape_comp = std::static_pointer_cast<ShapeComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterShapeComponent(shape_comp);
			break;
		}
		case eRenderType::kBillboard:
		{
			const auto& billboard_comp = std::static_pointer_cast<BillboardComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterBillboardComponent(billboard_comp);
			break;
		}
		case eRenderType::kWidget:
		{
			const auto& widget_comp = std::static_pointer_cast<WidgetComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterWidgetComponent(widget_comp);
			break;
		}
		case eRenderType::kLight:
		{
			const auto& light_comp = std::static_pointer_cast<LightComponent>(render_comp);
			const auto& local_light_comp = std::dynamic_pointer_cast<LocalLightComponent>(render_comp);
			if (local_light_comp != nullptr)
				m_graphics_shaders.at(shader_name)->UnregisterLocalLightComponent(local_light_comp);
			m_light_manager->UnregisterLightComponent(light_comp);
			break;
		}
		default:
			break;
		}
	}

	bool RenderSystem::RegisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		return m_graphics_shaders.at(shader_name)->RegisterSkyComponent(m_device, sky_comp);
	}

	void RenderSystem::UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return;
		}

		m_graphics_shaders.at(shader_name)->UnregisterSkyComponent(sky_comp);
	}

	bool RenderSystem::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp->GetCameraUsage() == eCameraUsage::kBasic)
			return m_render_camera_manager->RegisterCameraComponent(camera_comp);
		else
			return m_shadow_camera_manager->RegisterCameraComponent(camera_comp);
	}

	void RenderSystem::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp->GetCameraUsage() == eCameraUsage::kBasic)
			m_render_camera_manager->UnregisterCameraComponent(camera_comp);
		else
			m_shadow_camera_manager->UnregisterCameraComponent(camera_comp);
	}

	void RenderSystem::SetMainCamera(const SPtr<RenderCameraComponent>& camera_comp)
	{
		const auto& window = m_window.lock();
		m_render_camera_manager->SetMainCamera(camera_comp);
		m_render_camera_manager->UpdateMainCameraViewport(window->width, window->height);
	}
}
