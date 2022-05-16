#pragma once
#pragma warning(disable: 4819)
#include <filesystem>


#include <memory>

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace client_fw
{
	class Log
	{
	public:
		static void Initialize();

		inline static SPtr<spdlog::logger>& GetDefaultLogger() { return s_default_logger; }
		inline static SPtr<spdlog::logger>& GetTrackingLogger() { return s_tracking_logger; }
	private:
		static SPtr<spdlog::logger> s_default_logger;
		static SPtr<spdlog::logger> s_tracking_logger;
	};
}

//log macros
#ifdef _DEBUG
	#define LOG_TRACE(...)				client_fw::Log::GetDefaultLogger()->trace(__VA_ARGS__)
	#define LOG_INFO(...)				client_fw::Log::GetDefaultLogger()->info(__VA_ARGS__)
	#define LOG_WARN(...)				SPDLOG_LOGGER_WARN(client_fw::Log::GetTrackingLogger(), __VA_ARGS__)
	#define LOG_ERROR(...)				SPDLOG_LOGGER_ERROR(client_fw::Log::GetTrackingLogger(), __VA_ARGS__)
	#define LOG_CRITICAL(...)			SPDLOG_LOGGER_CRITICAL(client_fw::Log::GetTrackingLogger(), __VA_ARGS__)
#else
	#define LOG_TRACE(...)
	#define LOG_INFO(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_CRITICAL(...)
#endif



