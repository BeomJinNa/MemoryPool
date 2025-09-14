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

    // �ʱ� �޸� ����� �Ҵ��ϰ� FreeList�� ä���.
    // Grow()�� ���� �� ���α׷��� �����Ű�Ƿ�, ��ȯ ���� üũ�� �ʿ䰡 ����.
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

    // 1. ��κ��� ��츦 ���� ���� ���(Hot Path): �� ���� ť���� �ٷ� ������ �õ�
    if(mFreeList.try_pop(ptr))
    {
        return ptr;
    }

    // 2. ť�� ����ִ� �幮 ���(Cold Path): Ǯ Ȯ�� �õ�
    if(Grow())
    {
        // Ȯ�� �� �ٽ� ������ �õ�
        if(mFreeList.try_pop(ptr))
        {
            return ptr;
        }
    }

    // �� �ڵ�� �������� �����ؼ��� �� ��.
    // Grow()�� �����ϸ� �̹� ���α׷��� ����Ǿ��� ���̱� ����.
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

    // ���� ��ٸ��� ���� �ٸ� �����尡 �̹� Ǯ�� Ȯ������ �� �����Ƿ�,
    // ���� ȹ���� �� FreeList�� ������ ����ִ��� �ٽ� �ѹ� Ȯ���Ѵ�.
    if(!mFreeList.empty())
    {
        return true; // ����: �ٸ� �����尡 �̹� ó����
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

    // ���� Ȯ�� �ÿ��� ���� ũ���� 2��� �Ҵ�
    mNextBlockSize *= 2;

    return true;
}
} // namespace TinyMemoryPool::Detail