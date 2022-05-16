#pragma once

#include <memory>

namespace client_fw
{
	template<class T>
	using UPtr = std::unique_ptr<T>;
	template<class T, class ... Args>
	constexpr UPtr<T> CreateUPtr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using SPtr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr SPtr<T> CreateSPtr(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WPtr = std::weak_ptr<T>;	

	template<typename Enum>
	constexpr typename std::underlying_type<Enum>::type ToUnderlying(Enum e) noexcept
	{
		return static_cast<typename std::underlying_type<Enum>::type>(e);
	}
}

constexpr unsigned int HashCode(const char* str)
{
	return str[0] ? static_cast<unsigned int>(str[0]) + 0xEDB8832Full * HashCode(str + 1) : 8603;
}