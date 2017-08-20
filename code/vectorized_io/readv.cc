#include <iostream>
#include <sys/uio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


using std::cout;
using std::endl;


int main()
{
  //
  // test.txt:
  //   jackcatspizza
  //
  char str1[5] = { '\0' };
  char str2[5] = { '\0' };
  char str3[6] = { '\0' };
  iovec buffers[3];
  int fd;
  ssize_t bytesRead;
  
  // open file
  fd = open("test.txt", O_RDONLY);
  if (fd < 0)
  {
    cout << ":: unable to open file test.txt" << endl;
    return -1;
  }

  // initialize iovec
  buffers[0].iov_base = reinterpret_cast<void *>(str1);
  buffers[0].iov_len = sizeof(str1) - 1;
  buffers[1].iov_base = reinterpret_cast<void *>(str2);
  buffers[1].iov_len = sizeof(str2) - 1;
  buffers[2].iov_base = reinterpret_cast<void *>(str3);
  buffers[2].iov_len = sizeof(str3) - 1;

  // scatter read
  bytesRead = readv(fd, buffers, sizeof(buffers) / sizeof(*buffers));
  if (bytesRead < 0)
  {
    cout << ":: unable to read file" << endl;
    return -1;
  }

  // print data
  cout << str1 << endl;
  cout << str2 << endl;
  cout << str3 << endl;

  // close file
  close(fd);
  return 0;
}
