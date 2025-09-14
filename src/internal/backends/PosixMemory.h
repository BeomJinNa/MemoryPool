#pragma once
#if !defined(_WIN32)
#include <sys/mman.h>
#include <cstddef>
#include <unistd.h>
#include "Common.h"

namespace TinyMemoryPool::Detail
{
class PosixMemory
{
public:
	[[nodiscard]] static inline void* ReserveOrNull(std::size_t size) noexcept
	{
		void* ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		return (ptr == MAP_FAILED) ? nullptr : ptr;
	}

	static inline void Commit(void* ptr, std::size_t size) noexcept
	{
		int result = mprotect(ptr, size, PROT_READ | PROT_WRITE);
		if (result != 0) { TMP_FATAL_ERROR("mprotect failed!"); }
	}

	static inline void Release(void* ptr, std::size_t size) noexcept
	{
		int result = munmap(ptr, size);
		if (result != 0) { TMP_FATAL_ERROR("munmap failed!"); }
	}

	static inline std::size_t GetPageSize() noexcept
	{
		return static_cast<std::size_t>(sysconf(_SC_PAGESIZE));
	}

private:
	PosixMemory() = delete;
	~PosixMemory() = delete;
};
} // namespace TinyMemoryPool::Detail
#endif