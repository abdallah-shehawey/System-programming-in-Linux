#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
    exit(-1);
  }

  if (rename(argv[1], argv[2]) != 0)
  {
    fprintf(stderr, "mv: cannot move '%s' to '%s': %s\n", argv[1], argv[2], strerror(errno));
    exit(-2);
  }

  return 0;
}
