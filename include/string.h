#pragma once

#include_next <string.h>

#if !defined(__linux__) && !defined(__FreeBSD__)

static inline void *
mempcpy(void *dest, const void *src, size_t n)
{
  return (char *)memcpy(dest, src, n) + n;
}

#endif
