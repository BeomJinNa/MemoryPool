cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(TBB CONFIG REQUIRED)

add_library(TinyMemoryPool STATIC
        include/TinyMemoryPool/Config.h
        src/PoolAllocatorImpl.h
        src/internal/Common.h
        src/internal/MemoryManager.cpp
        src/internal/MemoryManager.h
        src/internal/MemoryManagerImpl.cpp
        src/internal/MemoryManagerImpl.h
        src/internal/PlatformMemory.h
        src/internal/Pool.cpp
        src/internal/Pool.h
        src/internal/backends/PosixMemory.h
        src/internal/backends/WindowsMemory.h
)

target_include_directories(TinyMemoryPool PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" # 내부 빌드용
    "$<INSTALL_INTERFACE:include>"                          # 외부 설치용
)

 target_link_libraries(TinyMemoryPool PRIVATE TBB::tbb)
