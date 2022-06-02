#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include "object/level/rendering/test/rotating_cube_level.h"
#include "object/level/rendering/basic/import_obj_mesh_level.h"
#include "object/level/rendering/basic/dynamic_lod_level.h"
#include "object/level/rendering/basic/instancing_level.h"
#include "object/level/rendering/basic/normal_map_level.h"
#include "object/level/rendering/basic/multi_camera_level.h"

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
			m_select_level_layer->RegisterLevel<RotatingCubeLevel>("Rendering/Test", { u8"회전하는 큐브가 있습니다." });
			m_select_level_layer->RegisterLevel<ImportObjMeshLevel>("Rendering/Basic", { u8"Obj mesh 파일을 Import 할 수 있습니다." });
			m_select_level_layer->RegisterLevel<DynamicLODLevel>("Rendering/Basic", { 
				u8"Static mesh의 Level Of Detail을 확인 할 수 있습니다.",
				u8"LOD는 최대 4까지(0~4) 지원합니다.",
				u8"LOD1 : 80%%, LOD2 : 50%%, LOD3 : 25%%, LOD4 : 10%%의 비율을 가집니다.",
				u8"카메라가 여러개라면 LOD값 출력이 제대로 동작하지 않습니다. (각 카메라에 대한 LOD는 잘 동작함)"
				});
			m_select_level_layer->RegisterLevel<InstancingLevel>("Rendering/Basic", { 
				u8"Hardware instancing을 사용해서 그리는 장면을 확인할 수 있습니다.",
				u8"초기 Actor의 수를 설정할 수 있습니다. (최소 1개 ~ 최대 125,000개)"
				});
			m_select_level_layer->RegisterLevel<NormalMapLevel>("Rendering/Basic", {
				u8"Normal map을 사용한 mesh와 사용하지 않은 mesh를 비교합니다.",
				});
			m_select_level_layer->RegisterLevel<MultiCameraLevel>("Rendering/Basic", {
				u8"여러개의 카메라가 존재합니다.",
				u8"각 카메라에서 바라보는 장면을 확인할 수 있습니다.",
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