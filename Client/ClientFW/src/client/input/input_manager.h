#pragma once

namespace client_fw
{
#define MAX_KEYS		256

	struct Window;

	class InputManager final
	{
	private:
		enum class EKeyState
		{
			kCur, kBefore, kConsumption
		};

		enum class EMousePosState
		{
			kCur, kBefore, kLastShow,
		};

	public:
		InputManager(const WPtr<Window>& window);
		~InputManager();

		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;
		
		void Update();
		void UpdateForNextFrame();

	public:
		bool IsKeyHoldDown(UINT key) const;
		bool IsKeyPressed(UINT key) const;
		bool IsKeyReleased(UINT key) const;
		bool IsNotKeyHoldDown(UINT key) const;
		const IVec2& GetMousePosition() const;
		const IVec2 GetRelativeMoustPosition() const;

		void ConsumeKey(UINT key);
		bool IsConsumedKey(UINT key) const;

		void SetHideCursor(bool hide);
		bool IsHideCursor() const { return m_is_hide_cursor; }

		void SetClipCursor(bool clip);
		bool IsClipCursor() const { return m_is_clip_cursor; }

		void StartInputMethodEditor();
		void EndInputMethodEditor();
		void OnChangeTextFromIME(const std::function<void(wchar_t)>& function);

	public:
		void ChangeKeyState(UINT message, WPARAM key, LPARAM flags);
		void ChangeMouseKeyState(int button, WPARAM wParam, int x, int y);
		void ChangeMouseMoveState(int button, WPARAM wParam, int x, int y);
		void ChangeIMEText(WPARAM wParam, LPARAM lParam);

	private:
		WPtr<Window> m_window;
		std::array<std::bitset<MAX_KEYS>, 3> m_key_states;
		std::array<IVec2, 3> m_mouse_position; // For the index, refer to EMousePosState
		HIMC m_himc = nullptr;
		std::function<void(wchar_t)> m_ime_function;

		bool m_is_track_window = false;
		bool m_is_hide_cursor = false;
		bool m_is_clip_cursor = false;
	};
}



