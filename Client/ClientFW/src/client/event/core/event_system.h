#pragma once

namespace client_fw
{
    struct Window;
    class InputManager;
    class InputEventManager;
    class UIEventManager;
    class MessageEventManager;
    class PacketEventManager;

    class EventSystem final
    {
    public:
        EventSystem(const WPtr<Window>& window);
        virtual ~EventSystem();

        EventSystem(const EventSystem&) = delete;
        EventSystem& operator=(const EventSystem&) = delete;

        void ExecuteEvent();
        void Update();
        void SendEventToServer();

    public:
        void ChangeInputState(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        static EventSystem* s_event_system;
        UPtr<InputManager> m_input_manager;
        UPtr<InputEventManager> m_input_event_manager;
        UPtr<UIEventManager> m_ui_event_manager;
        UPtr<MessageEventManager> m_message_event_manager;
        UPtr<PacketEventManager> m_packet_event_manager;

    public:
        static EventSystem& GetEventSystem() { return *s_event_system; }
        const UPtr<InputManager>& GetInputManager() const { return m_input_manager; }
        const UPtr<InputEventManager>& GetInputEventManager() const { return m_input_event_manager; }
        const UPtr<UIEventManager>& GetUIEventManager() const { return m_ui_event_manager; }
        const UPtr<MessageEventManager>& GetMessageEventManager() const { return m_message_event_manager; }
        const UPtr<PacketEventManager>& GetPacketEventManager() const { return m_packet_event_manager; }
    };
}



