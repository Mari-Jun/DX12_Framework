#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class UserInterfaceLayer;

	class UserInterfaceManager
	{
	public:
		UserInterfaceManager();
		virtual ~UserInterfaceManager() = default;

		UserInterfaceManager(const UserInterfaceManager&) = delete;
		UserInterfaceManager& operator=(const UserInterfaceManager&) = delete;

		virtual void Shutdown();
		virtual void Update(float delta_time);
		void Reset();

		bool RegisterUserInterfaceLayer(const SPtr<UserInterfaceLayer>& ui_layer);

	private:
		static UserInterfaceManager* s_ui_manager;
		bool m_is_updated_ui_layers = false;
		std::vector<SPtr<UserInterfaceLayer>> m_ui_layers;

	public:
		static UserInterfaceManager& GetUIManager() { return *s_ui_manager; }
		const std::vector<SPtr<UserInterfaceLayer>>& GetUILayers() const { return m_ui_layers; }
	};
}


