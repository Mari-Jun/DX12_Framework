#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace event_test
{
    using namespace client_fw;

    class EventTestUILayer : public UserInterfaceLayer
    {
    public:
        EventTestUILayer();
        virtual ~EventTestUILayer() = default;

    public:
        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;
    };

}



