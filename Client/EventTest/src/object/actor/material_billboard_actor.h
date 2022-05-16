#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
    class MaterialBillboardComponent;
}

namespace event_test
{
    using namespace client_fw;

    class MaterialBillboardActor final : public Actor
    {
    public:
    //private:
        MaterialBillboardActor(eMobilityState mobility, const std::string& path, const std::string& mtl_name, Vec2&& size, bool fix_up);
        virtual ~MaterialBillboardActor() = default;

        virtual bool Initialize() override;

    private:
        SPtr<MaterialBillboardComponent>  m_billboard_component;
        std::string m_path;
        std::string m_mtl_name;
        Vec2 m_size;
        bool m_fix_up;
    };
}



