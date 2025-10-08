This repository contains compatibility stuff for anything in C23
(e.g., `<stdbit.h>`) or POSIX (e.g., `posix_fallocate(3)`) that is
missing in missing in macOS and needed to compile the
[Monad Execution Events SDK](https://docs.monad.xyz/execution-events/).

These compatibility routines are taken from other upstream projects
(glibc, toybox, etc.) are bear the various original copyrights from the
source files in those projects.

The SDK is used for real-time data that reads from the execution daemon's
shared memory. The Category Labs monad node only runs on Linux, but the
event SDK snapshots can be used on macOS, either for development or for
offline analysis.
