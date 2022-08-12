#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include "object/level/rendering/test/rotating_cube_level.h"
#include "object/level/rendering/basic/import_obj_mesh_level.h"
#include "object/level/rendering/basic/dynamic_lod_level.h"
#include "object/level/rendering/basic/instancing_level.h"
#include "object/level/rendering/basic/normal_map_level.h"
#include "object/level/rendering/basic/multi_camera_level.h"
#include "object/level/rendering/basic/gbuffer_level.h"
#include "object/level/rendering/basic/billboard_level.h"
#include "object/level/rendering/basic/sky_level.h"

#include "object/level/rendering/lighting/light_level.h"

#include "object/level/rendering/postprocessing/blur_level.h"
#include "object/level/rendering/postprocessing/sobel_edge_level.h"

#include "object/level/component/player/possess_pawn_level.h"
#include "object/level/component/ui/ui_level.h"

#include "object/layer/imgui_demo_layer.h"
#include "object/layer/docking_layer.h"
#include "object/layer/viewport_layer.h"
#include "object/layer/select_level_layer.h"
#include "object/layer/level_setting_layer.h"
#include "object/layer/menu_layer.h"

using namespace client_fw;

namespace simulation
{
	class SimulationApp : public client_fw::Application
	{
	public:
		SimulationApp() : Application(L"Simulation App")
		{
			m_viewport_layer = CreateSPtr<ViewportLayer>();
			m_menu_layer = CreateSPtr<MenuLayer>();
			m_select_level_layer = CreateSPtr<SelectLevelLayer>();
			m_level_setting_layer = CreateSPtr<LevelSettingLayer>();
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			RegisterPressedEvent("Input Mode Game Only", std::vector{ EventKeyInfo{eKey::k1, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kGameOnly); return true;  });
			RegisterPressedEvent("Input Mode Game And UI", std::vector{ EventKeyInfo{eKey::k2, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIAndGame); return true;  });
			RegisterPressedEvent("Input Mode UI Only", std::vector{ EventKeyInfo{eKey::k3, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIOnly); return true;  });
			RegisterPressedEvent("Input Mode Editor Only", std::vector{ EventKeyInfo{eKey::k4, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kEditor); return true;  });

			//RegisterLayer(CreateSPtr<ImGuiDemoLayer>());
			RegisterLayer(CreateSPtr<DockingLayer>());
			RegisterLayer(m_viewport_layer);
			RegisterLayer(m_menu_layer);
			RegisterLayer(m_select_level_layer);
			RegisterLayer(m_level_setting_layer);

			m_menu_layer->OnResumeLevelEvent([this]()
				{
					const auto& level = LevelManager::GetLevelManager().GetCurrentLevel();
					if (level != nullptr)
					{
						level->SetLevelState(eLevelState::kInGame);
						m_menu_layer->SetIsLevelPuase(false);
					}
				});
			m_menu_layer->OnPauseLevelEvent([this]()
				{
					const auto& level = LevelManager::GetLevelManager().GetCurrentLevel();
					if (level != nullptr)
					{
						level->SetLevelState(eLevelState::kPaused);
						m_menu_layer->SetIsLevelPuase(true);
					}
				});
			m_menu_layer->OnCloseLevelEvent([this]()
				{
					LevelManager::GetLevelManager().CloseLevel();
					m_level_setting_layer->SetCurrentSimulationLevel(nullptr);
					m_menu_layer->SetIsLevelPuase(false);
				});
			m_menu_layer->OnShutdownEvent([this]() { SetAppState(eAppState::kDead); });

			m_select_level_layer->OnOpenEvent([this](const SPtr<SimulationLevel>& level)
				{
					m_level_setting_layer->SetCurrentSimulationLevel(level);
				});
			m_select_level_layer->OnCloseEvent([this](const SPtr<SimulationLevel>& level)
				{
					m_level_setting_layer->SetCurrentSimulationLevel(nullptr);
				});

			RegisterSimulationLevels();

			return result;
		}

		void RegisterSimulationLevels()
		{
			m_select_level_layer->RegisterLevel<RotatingCubeLevel>("Rendering/Test", { u8"ȸ���ϴ� ť�갡 �ֽ��ϴ�." });
			m_select_level_layer->RegisterLevel<ImportObjMeshLevel>("Rendering/Basic", { u8"Obj mesh ������ Import �� �� �ֽ��ϴ�." });
			m_select_level_layer->RegisterLevel<DynamicLODLevel>("Rendering/Basic", { 
				u8"Static mesh�� Level Of Detail�� Ȯ�� �� �� �ֽ��ϴ�.",
				u8"LOD�� �ִ� 4����(0~4) �����մϴ�.",
				u8"LOD1 : 80%%, LOD2 : 50%%, LOD3 : 25%%, LOD4 : 10%%�� ������ �����ϴ�.",
				u8"ī�޶� ��������� LOD�� ����� ����� �������� �ʽ��ϴ�. (�� ī�޶� ���� LOD�� �� ������)"
				});
			m_select_level_layer->RegisterLevel<InstancingLevel>("Rendering/Basic", { 
				u8"Hardware instancing�� ����ؼ� �׸��� ����� Ȯ���� �� �ֽ��ϴ�.",
				u8"�ʱ� Actor�� ���� ������ �� �ֽ��ϴ�. (�ּ� 1�� ~ �ִ� 125,000��)"
				});
			m_select_level_layer->RegisterLevel<NormalMapLevel>("Rendering/Basic", {
				u8"Normal map�� ����� mesh�� ������� ���� mesh�� ���մϴ�.",
				});
			m_select_level_layer->RegisterLevel<MultiCameraLevel>("Rendering/Basic", {
				u8"�������� ī�޶� �����մϴ�.",
				u8"�� ī�޶󿡼� �ٶ󺸴� ����� Ȯ���� �� �ֽ��ϴ�.",
				});
			m_select_level_layer->RegisterLevel<GBufferLevel>("Rendering/Basic", {
				u8"Deferred shading�� ���Ǵ� GBuffer�� Ȯ���� �� �ֽ��ϴ�.",
				});
			m_select_level_layer->RegisterLevel<TextureBillboardLevel>("Rendering/Basic", {
				u8"Texture Billboard Component�� ���� ��ü�� Ȯ���� �� �ֽ��ϴ�.",
				u8"Offset�� Tilling����� �����Ǿ� �ֽ��ϴ�.",
				u8"Billboard�� ũ�⸦ ������ �� �ֽ��ϴ�."
				});
			m_select_level_layer->RegisterLevel<TextureBillboardAnimationLevel>("Rendering/Basic", {
				u8"Texture Billboard Component�� �̿��Ͽ� ��������Ʈ �ִϸ��̼� ������ Ȯ���� �� �ֽ��ϴ�.",
				});
			m_select_level_layer->RegisterLevel<SkyCubeLevel>("Rendering/Basic", {
				u8"Sky Cube Texture�� ������ �� �ֽ��ϴ�.",
				u8"Cube map texture�� �ƴ� texture�� �ҷ����� �Ǹ� ���α׷��� ���� ���� �˴ϴ�."
				});

			m_select_level_layer->RegisterLevel<LightLevel>("Rendering/Lighting", {
				u8"�⺻���� Lighting ȿ������ Ȯ���� �� �ֽ��ϴ�.",
				u8"Light�� �����δ� Directional Light, Point Light, Spot Light�� �ֽ��ϴ�."
				});

			m_select_level_layer->RegisterLevel<BlurLevel>("Rendering/PostProcessing",{
				u8"Gaussian blur�� ����� blurring�� Ȯ���� �� �ֽ��ϴ�.",
				u8"Player�� �����ϴ� Pawn�� ������ Event�� ���� �ʽ��ϴ�. (ī�޶� ȸ��, �÷��̾� �̵�)"
				});
			m_select_level_layer->RegisterLevel<SobelEdgeLevel>("Rendering/PostProcessing", {
				u8"Sobel Edge�� ������ �������� Ȯ���� �� �ֽ��ϴ�.",
				});

			m_select_level_layer->RegisterLevel<PossessPawnLevel>("Component/Player", {
				u8"Camera pawn�� Possess �� �� �ֽ��ϴ�.",
				u8"�� Camera�� local tranform�� ���� �� �� �ֽ��ϴ�.",
				});

			m_select_level_layer->RegisterLevel<UILevel>("Component/User interface", {
				u8"�⺻���� �����Ǵ� HUD�� Ȯ���� �� �ֽ��ϴ�.",
				});
		}

		void Shutdown() override
		{
			Application::Shutdown();
		}

	private:
		SPtr<ViewportLayer> m_viewport_layer;
		SPtr<MenuLayer> m_menu_layer;
		SPtr<SelectLevelLayer> m_select_level_layer;
		SPtr<LevelSettingLayer> m_level_setting_layer;
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<simulation::SimulationApp>();
}