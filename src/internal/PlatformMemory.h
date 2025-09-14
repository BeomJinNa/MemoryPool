#pragma once
#include <cstddef>

#include "Common.h"
#include "backends/WindowsMemory.h"
#include "backends/PosixMemory.h"

#if defined(_WIN32)
#define PLATFORM_MEMORY_BACKEND TinyMemoryPool::Detail::WindowsMemory
#else
#define PLATFORM_MEMORY_BACKEND TinyMemoryPool::Detail::PosixMemory
#endif

namespace TinyMemoryPool::Detail
{
class PlatformMemory
{
public:
	/**
	 * @brief 가상 메모리 공간을 예약합니다. 실패 시 프로그램을 즉시 종료합니다.
	 * @param size 예약할 메모리의 바이트 크기.
	 * @return 성공 시 예약된 메모리 공간의 시작 주소.
	 */
	[[nodiscard]] static inline void* Reserve(std::size_t size) noexcept
	{
		void* ptr = PLATFORM_MEMORY_BACKEND::ReserveOrNull(size);
		if (ptr == nullptr)
		{
			TMP_FATAL_ERROR("PlatformMemory::Reserve failed!");
		}
		return ptr;
	}

	static inline void Commit(void* ptr, std::size_t size) noexcept
	{
		PLATFORM_MEMORY_BACKEND::Commit(ptr, size);
	}

	static inline void Release(void* ptr, std::size_t size) noexcept
	{
		PLATFORM_MEMORY_BACKEND::Release(ptr, size);
	}

	static inline std::size_t GetPageSize() noexcept
	{
		return PLATFORM_MEMORY_BACKEND::GetPageSize();
	}

private:
	PlatformMemory() = delete;
	~PlatformMemory() = delete;
};
} // namespace TinyMemoryPool::Detail