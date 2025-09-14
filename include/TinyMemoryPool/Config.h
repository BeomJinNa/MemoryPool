#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <cstddef>

namespace TinyMemoryPool
{
struct PoolConfig
{
	std::size_t chunkSize = 0;
	std::size_t initialBlockSize = 0;
};

struct MemoryManagerConfig
{
	std::size_t totalReserveSize = 1024 * 1024 * 1024;
	std::size_t frameAllocatorSize = 16 * 1024 * 1024;
	std::vector<PoolConfig> poolConfigs;
};

} // namespace TinyMemoryPool