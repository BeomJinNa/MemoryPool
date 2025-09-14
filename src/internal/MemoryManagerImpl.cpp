#include "MemoryManagerImpl.h"
#include "PlatformMemory.h"

namespace TinyMemoryPool
{

MemoryManagerImpl::MemoryManagerImpl()
{
}

MemoryManagerImpl::~MemoryManagerImpl()
{
	Shutdown();
}

void MemoryManagerImpl::Initialize(const MemoryManagerConfig& config)
{
	std::lock_guard<std::mutex> lock(mMutex);

	if (mIsInitialized)
	{
		TMP_FATAL_ERROR("MemoryManager is already initialized.");
		return;
	}

	// 설정값 저장
	mTotalReservedSize = config.totalReserveSize;

	// 가상 메모리 예약
	mReservedBaseAddress = Detail::PlatformMemory::Reserve(mTotalReservedSize);
	mPageSize = Detail::PlatformMemory::GetPageSize();

	// 예약 성공 후 오프셋 초기화
	mCurrentCommitOffset = 0;

	mIsInitialized = true;
}

void MemoryManagerImpl::Shutdown() noexcept
{
	std::lock_guard<std::mutex> lock(mMutex);

	if (!mIsInitialized)
	{
		return;
	}

	// 예약했던 모든 메모리 반납
	Detail::PlatformMemory::Release(mReservedBaseAddress, mTotalReservedSize);

	// 상태 초기화
	mPageSize = 0;
	mReservedBaseAddress = nullptr;
	mTotalReservedSize = 0;
	mCurrentCommitOffset = 0;
	mIsInitialized = false;
}

[[nodiscard]] void* MemoryManagerImpl::AllocateBlock(std::size_t size)
{
	std::lock_guard<std::mutex> lock(mMutex);

	TMP_ASSERT(mIsInitialized && "MemoryManager is not initialized.");

	// 페이지 크기에 맞춰 정렬
	const std::size_t pageSize = mPageSize;
	const std::size_t alignedSize = (size + pageSize - 1) & ~(pageSize - 1);

	// 남은 예약 공간이 충분한지 확인
	if (mCurrentCommitOffset + alignedSize > mTotalReservedSize)
	{
		TMP_FATAL_ERROR("Out of reserved memory.");
		return nullptr; // TMP_FATAL_ERROR가 종료시키므로 사실 도달하지 않음
	}

	// 커밋할 주소 계산
	void* commitAddress = static_cast<char*>(mReservedBaseAddress) + mCurrentCommitOffset;

	// 실제 메모리 커밋
	Detail::PlatformMemory::Commit(commitAddress, alignedSize);

	// 오프셋(마커)을 다음 위치로 이동
	mCurrentCommitOffset += alignedSize;

	return commitAddress;
}
} // namespace TinyMemoryPool