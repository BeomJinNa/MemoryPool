#pragma once
#include <cstddef>
#include <mutex>

#include <TinyMemoryPool/Config.h>

#include "Common.h"
#include "PlatformMemory.h"

namespace TinyMemoryPool
{
class MemoryManagerImpl
{
public:
	MemoryManagerImpl();
	~MemoryManagerImpl();

	void Initialize(const MemoryManagerConfig& config);
	void Shutdown() noexcept;

	[[nodiscard]] void* AllocateBlock(std::size_t size);

private:
	std::mutex mMutex;

	// 설정 및 상태
	bool mIsInitialized = false;

	// 저수준 메모리 관리 상태
	void* mReservedBaseAddress = nullptr;
	std::size_t mCurrentCommitOffset = 0;
	std::size_t mTotalReservedSize = 0;
	std::size_t mPageSize = 0;
};
} // namespace TinyMemoryPool