#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
    enum class eSkyType
    {
        kCube, kSphere
    };

    class SkyComponent : public Component, public std::enable_shared_from_this<SkyComponent>
    {
    protected:
        SkyComponent(eSkyType type, const std::string& draw_shader_name,
            const std::string& name = "sky component");
        virtual ~SkyComponent() = default;

        virtual bool InitializeComponent() override final;
        virtual void ShutdownComponent() override final;

    protected:
        eSkyType m_sky_type;
        std::string m_draw_shader_name;

    public:
        eSkyType GetSkyType() const { return m_sky_type; }
    };
}


