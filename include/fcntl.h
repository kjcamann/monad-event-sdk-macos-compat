#pragma once

#include_next <fcntl.h>

#ifdef __APPLE__
int posix_fallocate(int, off_t, off_t);
#endif
