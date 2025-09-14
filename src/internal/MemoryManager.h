#pragma once

#include <memory>
#include <cstddef>

#include <TinyMemoryPool/Config.h>

namespace TinyMemoryPool
{

class MemoryManagerImpl;

/**
 * @class MemoryManager
 * @brief TinyMemoryPool 라이브러리의 모든 기능을 총괄하는 중앙 관리자 클래스입니다.
 * Meyers' Singleton 패턴으로 구현되어, 프로그램 전역에서 단 하나의 인스턴스만 존재합니다.
 */
class MemoryManager
{
public:
	/**
	 * @brief MemoryManager의 유일한 인스턴스를 반환합니다.
	 * @return MemoryManager의 싱글턴 인스턴스에 대한 참조.
	 */
	static MemoryManager& GetInstance();

	/**
	 * @brief 메모리 관리 시스템을 초기화합니다. 프로그램 시작 시 반드시 한 번만 호출해야 합니다.
	 * @param config 메모리 풀의 구성 및 동작을 정의하는 설정 객체.
	 */
	void Initialize(const MemoryManagerConfig& config);

	/**
	 * @brief 메모리 관리 시스템을 종료하고 모든 예약된 메모리를 OS에 반납합니다.
	 * 프로그램 종료 시 소멸자에 의해 자동으로 호출됩니다.
	 */
	void Shutdown() noexcept;

	/**
	 * @brief 지정된 크기의 메모리를 할당합니다.
	 * 내부적으로 가장 적합한 풀을 찾거나, 크기가 큰 경우 폴백 할당을 사용합니다.
	 * @param size 할당할 메모리의 바이트 크기.
	 * @return 할당된 메모리의 시작 주소를 가리키는 포인터.
	 * @note 할당 실패 시 프로그램은 즉시 종료됩니다.
	 */
	[[nodiscard]] void* Allocate(std::size_t size);

	/**
	 * @brief 이전에 Allocate로 할당했던 메모리를 해제합니다.
	 * @param ptr 해제할 메모리의 포인터.
	 */
	void Deallocate(void* ptr) noexcept;

private:
	MemoryManager();
	~MemoryManager();

	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;
	MemoryManager(MemoryManager&&) = delete;
	MemoryManager& operator=(MemoryManager&&) = delete;

	std::unique_ptr<MemoryManagerImpl> mPimpl;
};

} // namespace TinyMemoryPool