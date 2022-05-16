#include "stdafx.h"
#include "client/core/timer.h"

namespace client_fw
{
	bool Timer::Initialize()
	{
		if (QueryPerformanceFrequency(static_cast<LARGE_INTEGER*>(&m_frequncy)))
		{
			QueryPerformanceCounter(&m_prev_time);
			m_time_scale = 1.0f / static_cast<float>(m_frequncy.QuadPart);

			return true;
		}
		else
		{
			LOG_CRITICAL("Could not support performance timer");
			return false;
		}
	}

	void Timer::Update(float lock_fps)
	{
		QueryPerformanceCounter(&m_cur_time);

		m_delta_time = (m_cur_time.QuadPart - m_prev_time.QuadPart)	* m_time_scale;

		if (lock_fps > 0.0f)
		{
			while (m_delta_time < (1.0f / lock_fps))
			{
				QueryPerformanceCounter(&m_cur_time);

				m_delta_time = (m_cur_time.QuadPart - m_prev_time.QuadPart)	* m_time_scale;
			}
		}
		m_prev_time = m_cur_time;

		++m_frame_count;
		m_fps_delta_time += m_delta_time;

		if (m_fps_delta_time >= 1.0f)
		{
			m_fps_delta_time -= 1.0f;
			m_frame_rate = m_frame_count;
			m_frame_count = 0;

			if (m_func_fps_changed != nullptr)
				m_func_fps_changed(m_frame_rate);
		}
	}

	void Timer::OnFpsChanged(const std::function<void(UINT)>& function)
	{
		m_func_fps_changed = function;
	}
}