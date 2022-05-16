#include "stdafx.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	UserInterfaceManager* UserInterfaceManager::s_ui_manager = nullptr;

	UserInterfaceManager::UserInterfaceManager()
	{
		s_ui_manager = this;
	}

	void UserInterfaceManager::Shutdown()
	{
		for (const auto& layer : m_ui_layers)
			layer->ShutdownUILayer();
		s_ui_manager = nullptr;
	}

	void UserInterfaceManager::Reset()
	{
		for (const auto& layer : m_ui_layers)
			layer->SetUILayerState(eUILayerState::kDead);
	}

	void UserInterfaceManager::Update(float delta_time)
	{
		for (auto ui = m_ui_layers.cbegin(); ui != m_ui_layers.cend();)
		{
			switch ((*ui)->GetUILayerState())
			{
			case eUILayerState::kActive:
			case eUILayerState::kHide:
				(*ui)->UpdateUILayer(delta_time);
				++ui;
				break;
			case eUILayerState::kDead:
				(*ui)->ShutdownUILayer();
				ui = m_ui_layers.erase(ui);
				break;
			default:
				break;
			}
		}
	}

	bool UserInterfaceManager::RegisterUserInterfaceLayer(const SPtr<UserInterfaceLayer>& ui_layer)
	{
		if (ui_layer->InitializeUILayer())
		{
			m_ui_layers.push_back(ui_layer);
			return true;
		}
		else
		{
			LOG_ERROR("Could not initialize user interface layer : {0}", ui_layer->GetName());
			ui_layer->ShutdownUILayer();
			return false;
		}
	}
}
