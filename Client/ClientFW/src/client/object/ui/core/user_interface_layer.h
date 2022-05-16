#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class UserInterface;

	enum class eUILayerState
	{
		kActive, kHide, kDead
	};

	class UserInterfaceLayer : public IBaseObject
	{
	protected:
		UserInterfaceLayer(const std::string& name = "user interface layer");
		virtual ~UserInterfaceLayer() = default;
	
	public:
		bool InitializeUILayer();
		virtual bool Initialize() override { return true; }
		void ShutdownUILayer();
		virtual void Shutdown() override {}
		void UpdateUILayer(float delta_time);
		virtual void Update(float delta_time) override {}

	protected:
		bool RegisterUserInterface(const SPtr<UserInterface>& ui);

	private:
		std::string m_name;
		eUILayerState m_layer_state;
		std::vector<SPtr<UserInterface>> m_user_interfaces;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		eUILayerState GetUILayerState() const { return m_layer_state; }
		void SetUILayerState(eUILayerState state) { m_layer_state = state; }
		const std::vector<SPtr<UserInterface>>& GetUserInterfaces() const { return m_user_interfaces; }
	};
}



