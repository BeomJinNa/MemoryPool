#include "MemoryManager.h"
#include "MemoryManagerImpl.h"

namespace TinyMemoryPool
{

MemoryManager& MemoryManager::GetInstance()
{
	static MemoryManager instance;
	return instance;
}

MemoryManager::MemoryManager() : mPimpl(std::make_unique<MemoryManagerImpl>()) {}

MemoryManager::~MemoryManager() = default;

void MemoryManager::Initialize(const MemoryManagerConfig& config)
{
	mPimpl->Initialize(config);
}

void MemoryManager::Shutdown() noexcept
{
	mPimpl->Shutdown();
}

void* MemoryManager::Allocate(std::size_t size)
{
	return mPimpl->Allocate(size);
}

void MemoryManager::Deallocate(void* ptr) noexcept
{
	mPimpl->Deallocate(ptr);
}
} // namespace TinyMemoryPool