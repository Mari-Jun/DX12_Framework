#pragma once
#include "client/event/core/event_manager.h"

namespace client_fw
{
    class UIEventInfo;

    class UIEventManager : public IEventManager
    {
    public:
        UIEventManager();
        virtual ~UIEventManager();

        UIEventManager(const UIEventManager&) = delete;
        UIEventManager& operator=(const UIEventManager&) = delete;

        virtual void ExecuteEvent() override;

    public:
        void RegisterEvent(UPtr<UIEventInfo>&& event_info);

    private:
        std::vector<UPtr<UIEventInfo>> m_ui_events;
    };
}


