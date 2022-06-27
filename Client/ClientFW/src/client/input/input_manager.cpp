#include "stdafx.h"
#include "client/input/input.h"
#include "client/input/input_manager.h"
#include "client/core/window.h"

namespace client_fw
{
    InputManager::InputManager(const WPtr<Window>& window)
        : m_window(window)
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::Update()
    {
        const auto& window = m_window.lock();

        if (m_is_hide_cursor)
        {
            POINT pos;
            GetCursorPos(&pos);
            ScreenToClient(window->hWnd, &pos);

            m_mouse_position[ToUnderlying(EMousePosState::kCur)] = IVec2(pos.x, pos.y);

            const IVec2& r_pos = GetRelativeMoustPosition();
            if (r_pos.x != 0) m_key_states[ToUnderlying(EKeyState::kCur)][ToUnderlying(eKey::kXMove)] = true;
            if (r_pos.y != 0) m_key_states[ToUnderlying(EKeyState::kCur)][ToUnderlying(eKey::kYMove)] = true;

            pos = window->mid_pos;
            ClientToScreen(window->hWnd, &pos);
            SetCursorPos(pos.x, pos.y);
        }
        else
        {
            POINT pos;
            GetCursorPos(&pos);
            ScreenToClient(window->hWnd, &pos);

            m_mouse_position[ToUnderlying(EMousePosState::kCur)] = IVec2(pos.x, pos.y) - window->viewport_pos;
        }
    }

    void InputManager::UpdateForNextFrame()
    {
        m_key_states[ToUnderlying(EKeyState::kBefore)] =
            m_key_states[ToUnderlying(EKeyState::kCur)];

        m_key_states[ToUnderlying(EKeyState::kConsumption)].reset();

        m_key_states[ToUnderlying(EKeyState::kCur)][ToUnderlying(eKey::kXMove)] =
            m_key_states[ToUnderlying(EKeyState::kCur)][ToUnderlying(eKey::kYMove)] = false;

        if (m_is_hide_cursor == false)
        {
            m_mouse_position[ToUnderlying(EKeyState::kBefore)] =
                m_mouse_position[ToUnderlying(EKeyState::kCur)];
        }
    }

    bool InputManager::IsKeyHoldDown(UINT key) const
    {
        return m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsKeyPressed(UINT key) const
    {
        return m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            !m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsKeyReleased(UINT key) const
    {
        return !m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    bool InputManager::IsNotKeyHoldDown(UINT key) const
    {
        return !m_key_states[ToUnderlying(EKeyState::kCur)][key] &&
            !m_key_states[ToUnderlying(EKeyState::kBefore)][key];
    }

    const IVec2& InputManager::GetMousePosition() const
    {
        return m_mouse_position[ToUnderlying(EMousePosState::kCur)];
    }

    const IVec2 InputManager::GetRelativeMoustPosition() const
    {
        return m_mouse_position[ToUnderlying(EMousePosState::kCur)] -
            m_mouse_position[ToUnderlying(EMousePosState::kBefore)];
    }

    void InputManager::ConsumeKey(UINT key)
    {
        m_key_states[ToUnderlying(EKeyState::kConsumption)][key] = true;
    }

    bool InputManager::IsConsumedKey(UINT key) const
    {
        return  m_key_states[ToUnderlying(EKeyState::kConsumption)][key];
    }

    void InputManager::SetHideCursor(bool hide)
    {
        if (hide)
        {
            POINT last_pos;
            GetCursorPos(&last_pos);
            ScreenToClient(m_window.lock()->hWnd, &last_pos);
            m_mouse_position[ToUnderlying(EMousePosState::kLastShow)] = IVec2(last_pos.x, last_pos.y);

            POINT pos = m_window.lock()->mid_pos;
            m_mouse_position[ToUnderlying(EMousePosState::kBefore)] =
                IVec2(pos.x, pos.y);

            SetClipCursor(true);
        }
        else
        {
            m_mouse_position[ToUnderlying(EMousePosState::kCur)] =
                m_mouse_position[ToUnderlying(EMousePosState::kBefore)] =
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)];

            POINT pos = {
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)].x,
                m_mouse_position[ToUnderlying(EMousePosState::kLastShow)].y
            };

            ClientToScreen(m_window.lock()->hWnd, &pos);
            SetCursorPos(pos.x, pos.y);
        }

        m_is_hide_cursor = hide;
        ShowCursor(!hide);
    }

    void InputManager::SetClipCursor(bool clip)
    {
        if (m_is_hide_cursor == false)
        {
            m_is_clip_cursor = clip;
            if (clip)
            {
                ClipCursor(&m_window.lock()->rect);
            }
            else
            {
                ClipCursor(nullptr);
            }
        }
    }

    void InputManager::StartInputMethodEditor()
    {
        if (m_himc == nullptr)
        {
            m_himc = ImmCreateContext();
            ImmAssociateContext(m_window.lock()->hWnd, m_himc);
        }
    }

    void InputManager::EndInputMethodEditor()
    {
        if (m_himc != nullptr)
        {
            ImmDestroyContext(m_himc);
            m_himc = nullptr;
        }
    }

    void InputManager::OnChangeTextFromIME(const std::function<void(wchar_t)>& function)
    {
        m_ime_function = function;
    }

    void InputManager::ChangeKeyState(UINT message, WPARAM key, LPARAM flags)
    {
        bool pressed = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
        m_key_states[ToUnderlying(EKeyState::kCur)].set(key, pressed);
    }

    void InputManager::ChangeMouseKeyState(int button, WPARAM wParam, int x, int y)
    {
        bool down = false;
        HWND hWnd = m_window.lock()->hWnd;
        eKey key;

        switch (button)
        {
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            key = eKey::kLButton;
            down = true;
            break;
        case WM_LBUTTONUP:
            ReleaseCapture();
            key = eKey::kLButton;
            down = false;
            break;
        case WM_RBUTTONDOWN:
            SetCapture(hWnd);
            key = eKey::kRButton;
            down = true;
            break;
        case WM_RBUTTONUP:
            ReleaseCapture();
            key = eKey::kRButton;
            down = false;
            break;
        case WM_MBUTTONDOWN:
            SetCapture(hWnd);
            key = eKey::kMButton;
            down = true;
            break;
        case WM_MBUTTONUP:
            ReleaseCapture();
            key = eKey::kMButton;
            down = false;
            break;
        }

        m_key_states[ToUnderlying(EKeyState::kCur)][ToUnderlying(key)] = down;
    }

    void InputManager::ChangeMouseMoveState(int button, WPARAM wParam, int x, int y)
    {
        HWND hWnd = m_window.lock()->hWnd;

        switch (button)
        {
        case WM_MOUSEMOVE:
        {
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = hWnd;
            tme.dwFlags = TME_LEAVE | TME_HOVER;
            tme.dwHoverTime = 1;
            TrackMouseEvent(&tme);
            break;
        }
        case WM_MOUSEHOVER:
            m_is_track_window = true;
            break;
        case WM_MOUSELEAVE:
            m_is_track_window = false;
            break;
        }
    }

    void InputManager::ChangeIMEText(WPARAM wParam, LPARAM lParam)
    {
        if (m_ime_function != nullptr)
            m_ime_function(static_cast<wchar_t>(wParam));
    }
}