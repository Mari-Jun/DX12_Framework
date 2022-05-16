#include <include/client_fw.h>
//#include <client/core/application.h>
//#include <client/core/log.h>
//#include <client/input/input.h>
#include <client/core/entry_point.h>
#include "object/level/client_test_level.h"

using namespace client_fw;

namespace client_test
{
	class ClientTestApp : public client_fw::Application
	{
	public:
		ClientTestApp() : Application(L"Client Test App")
		{
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			if (result)
			{
				LOG_INFO("Welcome to Client Test App");
			}

			RegisterPressedEvent("Clip Cursor", std::vector{ EventKeyInfo{eKey::kF3, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetClipCursor(!Input::IsClipCursor()); return true;  });
			RegisterPressedEvent("Hide Cursor", std::vector{ EventKeyInfo{eKey::kF2, {eAdditionalKey::kControl}} },
				[]()->bool {Input::SetHideCursor(!Input::IsHideCursor()); return true;  });
			RegisterPressedEvent("Shutdown App", std::vector{ EventKeyInfo{eKey::kF4, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[this]()->bool {SetAppState(eAppState::kDead); return true;  });

			RegisterPressedEvent("Open Level", { EventKeyInfo{eKey::kZ} },
				[this]()->bool {
					auto level = CreateSPtr<ClientTestLevel>(); 
					OpenLevel(level); return true;
				});

			RegisterPressedEvent("Close Level", { EventKeyInfo{eKey::kX} },
				[this]()->bool {
					CloseLevel(); return true;
				});

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();

			LOG_INFO("Good Bye");
		}

		virtual ~ClientTestApp()
		{
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<client_test::ClientTestApp>();
}