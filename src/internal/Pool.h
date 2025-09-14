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
 * @brief 단일 크기의 청크(Chunk)들을 관리하는 내부 클래스입니다.
 * 이 클래스는 스레드로부터의 동시 확장 요청에 안전하도록 설계되었습니다.
 */
class Pool
{
  public:
    /**
     * @brief Pool을 초기화합니다.
     * @param chunkSize 이 풀이 관리할 각 청크의 바이트 크기.
     * @param initialBlockSize 최초에 할당받을 메모리 블록의 전체 크기.
     */
    void Initialize(std::size_t chunkSize, std::size_t initialBlockSize);

    /**
     * @brief Pool을 종료하고 내부 상태를 정리합니다.
     */
    void Shutdown() noexcept;

    /**
     * @brief 가용 청크 리스트(Free List)에서 메모리 청크 하나를 가져옵니다.
     * @return 사용 가능한 메모리 청크의 유효한 포인터. (실패 시 프로그램 종료)
     */
    [[nodiscard]] void* Pop();

    /**
     * @brief 사용이 끝난 메모리 청크를 가용 청크 리스트에 반납합니다.
     * @param ptr 반납할 메모리 청크의 포인터.
     */
    void Push(void* ptr);

    /**
     * @brief 이 풀이 관리하는 청크의 크기를 반환합니다.
     * @return 청크의 바이트 크기.
     */
    std::size_t GetChunkSize() const noexcept;

  private:
    /**
     * @brief 가용 청크가 부족할 때 새로운 메모리 블록을 할당받아 풀을 확장합니다.
     * @return 확장 성공 여부. (실패 시 프로그램 종료)
     */
    bool Grow();

    std::size_t mChunkSize = 0;
    std::size_t mNextBlockSize = 0; // 다음 확장 시 할당할 블록 크기

    tbb::concurrent_queue<void*> mFreeList;

    std::vector<void*> mMemoryBlocks;
    std::mutex mGrowMutex;
};

} // namespace TinyMemoryPool::Detail