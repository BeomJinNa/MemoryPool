#pragma once

#include <iostream>

#if !defined(NDEBUG)
#include <cassert>
#define TMP_ASSERT(condition) assert(condition)
#else
#define TMP_ASSERT(condition)
#endif

// 릴리스 빌드에서도 동작하는 치명적 오류 처리 매크로
#define TMP_FATAL_ERROR(message) std::terminate()