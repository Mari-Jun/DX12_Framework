#pragma once
#include <client/object/layer/core/layer.h>

namespace simulation
{
	using namespace client_fw;

	class MenuLayer : public Layer
	{
	public:
		MenuLayer();
		virtual ~MenuLayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		bool m_is_level_pause = false;

	public:
		void SetIsLevelPuase(bool pause) { m_is_level_pause = pause; }

	private:
		SPtr<Texture> m_log_texture;
		SPtr<Texture> m_resume_level_texture;
		SPtr<Texture> m_pause_level_texture;
		SPtr<Texture> m_close_level_texture;
		SPtr<Texture> m_shutdown_texture;

	private:
		std::function<void()> m_resume_level_event;
		std::function<void()> m_pause_level_event;
		std::function<void()> m_close_level_event;
		std::function<void()> m_shutdown_event;

	public:
		void OnResumeLevelEvent(std::function<void()>&& function) { m_resume_level_event = function; }
		void OnPauseLevelEvent(std::function<void()>&& function) { m_pause_level_event = function; }
		void OnCloseLevelEvent(std::function<void()>&& function) { m_close_level_event = function; }
		void OnShutdownEvent(std::function<void()>&& function) { m_shutdown_event = function; }
	};
}


