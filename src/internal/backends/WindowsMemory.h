#pragma once

#if defined(_WIN32)

#include <Windows.h>
#include <cstddef>

#include "Common.h"

namespace TinyMemoryPool::Detail
{
class WindowsMemory
{
public:
	[[nodiscard]] static inline void* ReserveOrNull(std::size_t size) noexcept
	{
		return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);
	}

	static inline void Commit(void* ptr, std::size_t size) noexcept
	{
		void* result = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
		if (result == nullptr) { TMP_FATAL_ERROR("VirtualAlloc commit failed!"); }
	}

	static inline void Release(void* ptr, std::size_t size) noexcept
	{
		BOOL success = VirtualFree(ptr, 0, MEM_RELEASE);
		if (success == FALSE) { TMP_FATAL_ERROR("VirtualFree release failed!"); }
	}

	static inline std::size_t GetPageSize() noexcept
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		return sysInfo.dwPageSize;
	}

private:
	WindowsMemory() = delete;
	~WindowsMemory() = delete;
};
} // namespace TinyMemoryPool::Detail
#endif