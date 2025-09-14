#pragma once

#include <iostream>

#if !defined(NDEBUG)
#include <cassert>
#define TMP_ASSERT(condition) assert(condition)
#else
#define TMP_ASSERT(condition)
#endif

// ������ ���忡���� �����ϴ� ġ���� ���� ó�� ��ũ��
#define TMP_FATAL_ERROR(message) std::terminate()