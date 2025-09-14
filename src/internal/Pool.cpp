#include "Pool.h"

#include <cstddef>

#include "Common.h"
#include "MemoryManager.h"

namespace TinyMemoryPool::Detail
{
void Pool::Initialize(std::size_t chunkSize, std::size_t initialBlockSize)
{
    mChunkSize = chunkSize;
    mNextBlockSize = initialBlockSize;

    // 초기 메모리 블록을 할당하고 FreeList를 채운다.
    // Grow()는 실패 시 프로그램을 종료시키므로, 반환 값을 체크할 필요가 없음.
    Grow();
}

void Pool::Shutdown() noexcept
{
    mFreeList.clear();
    mMemoryBlocks.clear();
}

[[nodiscard]] void* Pool::Pop()
{
    void* ptr = nullptr;

    // 1. 대부분의 경우를 위한 빠른 경로(Hot Path): 락 없이 큐에서 바로 꺼내기 시도
    if(mFreeList.try_pop(ptr))
    {
        return ptr;
    }

    // 2. 큐가 비어있는 드문 경우(Cold Path): 풀 확장 시도
    if(Grow())
    {
        // 확장 후 다시 꺼내기 시도
        if(mFreeList.try_pop(ptr))
        {
            return ptr;
        }
    }

    // 이 코드는 논리적으로 도달해서는 안 됨.
    // Grow()가 실패하면 이미 프로그램이 종료되었을 것이기 때문.
    TMP_FATAL_ERROR("Failed to pop from pool after growing. This should not happen.");
    return nullptr;
}

void Pool::Push(void* ptr)
{
    mFreeList.push(ptr);
}

std::size_t Pool::GetChunkSize() const noexcept
{
    return mChunkSize;
}

bool Pool::Grow()
{
    std::lock_guard<std::mutex> lock(mGrowMutex);

    // 락을 기다리는 동안 다른 스레드가 이미 풀을 확장했을 수 있으므로,
    // 락을 획득한 후 FreeList가 여전히 비어있는지 다시 한번 확인한다.
    if(!mFreeList.empty())
    {
        return true; // 성공: 다른 스레드가 이미 처리함
    }

    void* newBlock = TinyMemoryPool::MemoryManager::GetInstance().Allocate(mNextBlockSize);

    mMemoryBlocks.push_back(newBlock);

    const std::size_t numChunks = mNextBlockSize / mChunkSize;
    auto currentChunk = static_cast<std::byte*>(newBlock);

    for(std::size_t i = 0; i < numChunks; ++i)
    {
        mFreeList.push(currentChunk);
        currentChunk += mChunkSize;
    }

    // 다음 확장 시에는 현재 크기의 2배로 할당
    mNextBlockSize *= 2;

    return true;
}
} // namespace TinyMemoryPool::Detail