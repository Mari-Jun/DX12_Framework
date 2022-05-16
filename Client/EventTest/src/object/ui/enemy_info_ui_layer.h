#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
    class TextUI;
}

namespace event_test
{
    using namespace client_fw;

    class RotatingCube;

    class EnemyInfoUILayer : public UserInterfaceLayer
    {
    public:
        EnemyInfoUILayer();
        virtual ~EnemyInfoUILayer() = default;

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    private:
        SPtr<TextUI> m_text;
        WPtr<RotatingCube> m_cube;

    public:
        void SetRotatingCube(const WPtr<RotatingCube>& owner) { m_cube = owner; }
    };
}



