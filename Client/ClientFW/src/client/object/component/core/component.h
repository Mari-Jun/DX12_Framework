#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Actor;

	class Component : public IBaseObject
	{
	protected:
		Component(const std::string& name, int update_order = 100);
		virtual ~Component() = default;

	public:
		virtual bool InitializeComponent();
		virtual bool Initialize() override { return true; }

		virtual void ShutdownComponent();
		virtual void Shutdown() override {}

		virtual void UpdateComponent(float delta_time);
		virtual void Update(float delta_time) override {}

		virtual void UpdateWorldMatrix() {}

	protected:
		std::string m_name;
		int m_update_order;
		WPtr<Actor> m_owner;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		int GetUpdateOrder() const { return m_update_order; }
		const WPtr<Actor>& GetOwner() const { return m_owner; }
		void SetOwner(const WPtr<Actor>& owner) { m_owner = owner; }
	};
}


