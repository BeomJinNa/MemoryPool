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

	// ������ ����
	mTotalReservedSize = config.totalReserveSize;

	// ���� �޸� ����
	mReservedBaseAddress = Detail::PlatformMemory::Reserve(mTotalReservedSize);
	mPageSize = Detail::PlatformMemory::GetPageSize();

	// ���� ���� �� ������ �ʱ�ȭ
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

	// �����ߴ� ��� �޸� �ݳ�
	Detail::PlatformMemory::Release(mReservedBaseAddress, mTotalReservedSize);

	// ���� �ʱ�ȭ
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

	// ������ ũ�⿡ ���� ����
	const std::size_t pageSize = mPageSize;
	const std::size_t alignedSize = (size + pageSize - 1) & ~(pageSize - 1);

	// ���� ���� ������ ������� Ȯ��
	if (mCurrentCommitOffset + alignedSize > mTotalReservedSize)
	{
		TMP_FATAL_ERROR("Out of reserved memory.");
		return nullptr; // TMP_FATAL_ERROR�� �����Ű�Ƿ� ��� �������� ����
	}

	// Ŀ���� �ּ� ���
	void* commitAddress = static_cast<char*>(mReservedBaseAddress) + mCurrentCommitOffset;

	// ���� �޸� Ŀ��
	Detail::PlatformMemory::Commit(commitAddress, alignedSize);

	// ������(��Ŀ)�� ���� ��ġ�� �̵�
	mCurrentCommitOffset += alignedSize;

	return commitAddress;
}
} // namespace TinyMemoryPool