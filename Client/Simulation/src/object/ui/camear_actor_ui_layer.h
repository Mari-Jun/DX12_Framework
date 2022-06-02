#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
    class TextUI;
}

namespace simulation
{
    using namespace client_fw;

    class CameraActor;

    class CameraActorUILayer : public UserInterfaceLayer
    {
    public:
        CameraActorUILayer();
        virtual ~CameraActorUILayer() = default;

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    private:
        SPtr<TextUI> m_text;
        WPtr<CameraActor> m_owner;

    public:
        void SetCameraActor(const WPtr<CameraActor>& owner) { m_owner = owner; }
    };
}



