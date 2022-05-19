#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include "object/level/rotating_cube_level.h"

#include "object/layer/imgui_demo_layer.h"
#include "object/layer/docking_layer.h"
#include "object/layer/viewport_layer.h"
#include "object/layer/select_level_layer.h"

using namespace client_fw;

namespace simulation
{
	class SimulationApp : public client_fw::Application
	{
	public:
		SimulationApp() : Application(L"Simulation App")
		{
			m_select_level_layer = CreateSPtr<SelectLevelLayer>();
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			RegisterPressedEvent("Input Mode Game Only", std::vector{ EventKeyInfo{eKey::k1, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kGameOnly); return true;  });
			RegisterPressedEvent("Input Mode Game And UI", std::vector{ EventKeyInfo{eKey::k2, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIAndGame); return true;  });
			RegisterPressedEvent("Input Mode UI Only", std::vector{ EventKeyInfo{eKey::k3, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIOnly); return true;  });

			//RegisterLayer(CreateSPtr<ImGuiDemoLayer>());
			RegisterLayer(CreateSPtr<DockingLayer>());
			RegisterLayer(CreateSPtr<ViewportLayer>());
			RegisterLayer(m_select_level_layer);

			m_select_level_layer->RegisterLevel<RotatingCubeLevel>({ "open rotating cube level" });

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();
		}

	private:
		SPtr<SelectLevelLayer> m_select_level_layer;
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<simulation::SimulationApp>();
}