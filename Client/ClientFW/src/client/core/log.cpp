#include "stdafx.h"
#include "client/core/log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace client_fw
{
	SPtr<spdlog::logger> Log::s_default_logger;
	SPtr<spdlog::logger> Log::s_tracking_logger;

	void Log::Initialize()
	{
		std::vector<spdlog::sink_ptr> default_sinks;
		default_sinks.emplace_back(CreateSPtr<spdlog::sinks::stderr_color_sink_mt>());

		default_sinks[0]->set_pattern("%v%$");

		std::vector<spdlog::sink_ptr> tracking_sinks;
		tracking_sinks.emplace_back(CreateSPtr<spdlog::sinks::wincolor_stderr_sink_mt>());
		tracking_sinks.emplace_back(CreateSPtr<spdlog::sinks::basic_file_sink_mt>("client.log", true));

		tracking_sinks[0]->set_pattern("[%s] [%!] [line : %#] : %v%$");

		s_default_logger = CreateSPtr<spdlog::logger>("Default", begin(default_sinks), end(default_sinks));
		spdlog::register_logger(s_default_logger);
		s_default_logger->set_level(spdlog::level::trace);

		s_tracking_logger = CreateSPtr<spdlog::logger>("Tracking", begin(tracking_sinks), end(tracking_sinks));
		spdlog::register_logger(s_tracking_logger);
		s_tracking_logger->set_level(spdlog::level::warn);
	}
}