#pragma once

namespace client_fw
{
	class Timer final
	{
	public:
		Timer() = default;

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		bool Initialize();
		void Update(float lock_fps = 60.0f);

		void OnFpsChanged(const std::function<void(UINT)>& function);

	private:
		LARGE_INTEGER m_frequncy = { 0 };
		LARGE_INTEGER m_cur_time = { 0 };
		LARGE_INTEGER m_prev_time = { 0 };
		UINT m_frame_rate = 0;
		UINT m_frame_count = 0;
		float m_time_scale = 0.0f;
		float m_delta_time = 0.0f;
		float m_fps_delta_time = 0.0f;

		std::function<void(UINT)> m_func_fps_changed = nullptr;

	public:
		UINT GetFrameRate() const { return m_frame_rate; }
		float GetDeltaTime() const { return m_delta_time; }
	};
}



