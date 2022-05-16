#pragma once
#include <client/object/component/core/component.h>

namespace client_test
{
	using namespace client_fw;

	class ClientTestComponent final : public Component
	{
	public:
		ClientTestComponent(const std::string& name, int update_order);
		virtual ~ClientTestComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;
	};

}


