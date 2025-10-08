#pragma once

#if __has_include_next(<stdbit.h>)
#include <stdbit.h>
#else

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#include "monad_compat/common/stdbit.h"
#pragma clang diagnostic pop

#endif
