#pragma once

#include <memory>
#include <cstddef>

#include <TinyMemoryPool/Config.h>

namespace TinyMemoryPool
{

class MemoryManagerImpl;

/**
 * @class MemoryManager
 * @brief TinyMemoryPool ���̺귯���� ��� ����� �Ѱ��ϴ� �߾� ������ Ŭ�����Դϴ�.
 * Meyers' Singleton �������� �����Ǿ�, ���α׷� �������� �� �ϳ��� �ν��Ͻ��� �����մϴ�.
 */
class MemoryManager
{
public:
	/**
	 * @brief MemoryManager�� ������ �ν��Ͻ��� ��ȯ�մϴ�.
	 * @return MemoryManager�� �̱��� �ν��Ͻ��� ���� ����.
	 */
	static MemoryManager& GetInstance();

	/**
	 * @brief �޸� ���� �ý����� �ʱ�ȭ�մϴ�. ���α׷� ���� �� �ݵ�� �� ���� ȣ���ؾ� �մϴ�.
	 * @param config �޸� Ǯ�� ���� �� ������ �����ϴ� ���� ��ü.
	 */
	void Initialize(const MemoryManagerConfig& config);

	/**
	 * @brief �޸� ���� �ý����� �����ϰ� ��� ����� �޸𸮸� OS�� �ݳ��մϴ�.
	 * ���α׷� ���� �� �Ҹ��ڿ� ���� �ڵ����� ȣ��˴ϴ�.
	 */
	void Shutdown() noexcept;

	/**
	 * @brief ������ ũ���� �޸𸮸� �Ҵ��մϴ�.
	 * ���������� ���� ������ Ǯ�� ã�ų�, ũ�Ⱑ ū ��� ���� �Ҵ��� ����մϴ�.
	 * @param size �Ҵ��� �޸��� ����Ʈ ũ��.
	 * @return �Ҵ�� �޸��� ���� �ּҸ� ����Ű�� ������.
	 * @note �Ҵ� ���� �� ���α׷��� ��� ����˴ϴ�.
	 */
	[[nodiscard]] void* Allocate(std::size_t size);

	/**
	 * @brief ������ Allocate�� �Ҵ��ߴ� �޸𸮸� �����մϴ�.
	 * @param ptr ������ �޸��� ������.
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