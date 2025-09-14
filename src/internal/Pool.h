#pragma once

#include <cstddef>
#include <mutex>
#include <vector>

#include <tbb/concurrent_queue.h>

namespace TinyMemoryPool::Detail
{
class MemoryManager;

/**
 * @class Pool
 * @brief ���� ũ���� ûũ(Chunk)���� �����ϴ� ���� Ŭ�����Դϴ�.
 * �� Ŭ������ ������κ����� ���� Ȯ�� ��û�� �����ϵ��� ����Ǿ����ϴ�.
 */
class Pool
{
  public:
    /**
     * @brief Pool�� �ʱ�ȭ�մϴ�.
     * @param chunkSize �� Ǯ�� ������ �� ûũ�� ����Ʈ ũ��.
     * @param initialBlockSize ���ʿ� �Ҵ���� �޸� ����� ��ü ũ��.
     */
    void Initialize(std::size_t chunkSize, std::size_t initialBlockSize);

    /**
     * @brief Pool�� �����ϰ� ���� ���¸� �����մϴ�.
     */
    void Shutdown() noexcept;

    /**
     * @brief ���� ûũ ����Ʈ(Free List)���� �޸� ûũ �ϳ��� �����ɴϴ�.
     * @return ��� ������ �޸� ûũ�� ��ȿ�� ������. (���� �� ���α׷� ����)
     */
    [[nodiscard]] void* Pop();

    /**
     * @brief ����� ���� �޸� ûũ�� ���� ûũ ����Ʈ�� �ݳ��մϴ�.
     * @param ptr �ݳ��� �޸� ûũ�� ������.
     */
    void Push(void* ptr);

    /**
     * @brief �� Ǯ�� �����ϴ� ûũ�� ũ�⸦ ��ȯ�մϴ�.
     * @return ûũ�� ����Ʈ ũ��.
     */
    std::size_t GetChunkSize() const noexcept;

  private:
    /**
     * @brief ���� ûũ�� ������ �� ���ο� �޸� ����� �Ҵ�޾� Ǯ�� Ȯ���մϴ�.
     * @return Ȯ�� ���� ����. (���� �� ���α׷� ����)
     */
    bool Grow();

    std::size_t mChunkSize = 0;
    std::size_t mNextBlockSize = 0; // ���� Ȯ�� �� �Ҵ��� ��� ũ��

    tbb::concurrent_queue<void*> mFreeList;

    std::vector<void*> mMemoryBlocks;
    std::mutex mGrowMutex;
};

} // namespace TinyMemoryPool::Detail