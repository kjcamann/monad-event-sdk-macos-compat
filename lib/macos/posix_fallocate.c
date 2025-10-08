#include <errno.h>
#include <fcntl.h>
#include <sys/disk.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Copied from toybox's portability.c
 */

#define maxof(a, b) ({typeof(a) aa = (a); typeof(b) bb = (b); aa>bb ? aa : bb;})

static int get_block_device_size(int fd, unsigned long long* size)
{
  unsigned long block_size, block_count;

  if (!ioctl(fd, DKIOCGETBLOCKSIZE, &block_size) &&
      !ioctl(fd, DKIOCGETBLOCKCOUNT, &block_count)) {
    *size = block_count * block_size;
    return 1;
  }
  return 0;
}

// Return how long the file at fd is, if there's any way to determine it.
static off_t fdlength(int fd)
{
  struct stat st;
  off_t base = 0, range = 1, expand = 1, old;
  unsigned long long size;

  if (!fstat(fd, &st) && S_ISREG(st.st_mode)) return st.st_size;

  // If the ioctl works for this, return it.
  if (get_block_device_size(fd, &size)) return size;

  // If not, do a binary search for the last location we can read.  (Some
  // block devices don't do BLKGETSIZE right.)  This should probably have
  // a CONFIG option...
  old = lseek(fd, 0, SEEK_CUR);
  do {
    char temp;
    off_t pos = base + range / 2;

    if (lseek(fd, pos, 0)>=0 && read(fd, &temp, 1)==1) {
      off_t delta = (pos + 1) - base;

      base += delta;
      if (expand) range = (expand <<= 1) - base;
      else range -= delta;
    } else {
      expand = 0;
      range = pos - base;
    }
  } while (range > 0);

  lseek(fd, old, SEEK_SET);

  return base;
}

// As of 10.15, macOS offers an fcntl F_PREALLOCATE rather than fallocate()
// or posix_fallocate() calls. The fcntl only (as the name implies)
// pre-allocates, so we also need to ftruncate() afterwards.
int posix_fallocate(int fd, off_t offset, off_t length)
{
  int e = errno, result = 0;
  fstore_t f;

  f.fst_flags = F_ALLOCATEALL;
  f.fst_posmode = F_PEOFPOSMODE;
  f.fst_offset = 0;
  f.fst_length = offset + length;
  if (fcntl(fd, F_PREALLOCATE, &f) == -1) result = errno;
  else if (ftruncate(fd, maxof(offset+length, fdlength(fd)))) result = errno;
  errno = e;
  return result;
}
