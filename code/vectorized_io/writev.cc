#include <iostream>
#include <sys/uio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


using std::cout;
using std::endl;


int main()
{
  char *str1 = "jack";
  char *str2 = "cats";
  char *str3 = "pizza";
  iovec buffers[3];
  int fd;
  ssize_t writtenBytes;

  // open file
  fd = open("test.txt", O_CREAT | O_TRUNC | O_WRONLY);
  if (fd < 0) 
  {
    cout << ":: unable to open file test.txt" << endl;
    return -1;
  }

  // intialize iovec
  buffers[0].iov_base = reinterpret_cast<void *>(str1);
  buffers[0].iov_len = strlen(str1);
  buffers[1].iov_base = reinterpret_cast<void *>(str2);
  buffers[1].iov_len = strlen(str2);
  buffers[2].iov_base = reinterpret_cast<void *>(str3);
  buffers[2].iov_len = strlen(str3);

  // write
  writtenBytes = writev(fd, buffers, sizeof(buffers) / sizeof(*buffers));
  if (writtenBytes < 0)
  {
    cout << ":: failed to write to file" << endl;
    return -1;
  }

  // close file
  close(fd);
  return 0;
}
