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

	// ���� �� ����
	bool mIsInitialized = false;

	// ������ �޸� ���� ����
	void* mReservedBaseAddress = nullptr;
	std::size_t mCurrentCommitOffset = 0;
	std::size_t mTotalReservedSize = 0;
	std::size_t mPageSize = 0;
};
} // namespace TinyMemoryPool