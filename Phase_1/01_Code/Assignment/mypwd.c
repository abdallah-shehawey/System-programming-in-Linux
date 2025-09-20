#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void main()
{
  char buffer[PATH_MAX];

  if (getcwd(buffer, sizeof(buffer)) == NULL)
  {
    fprintf(stderr, "pwd: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  printf("%s\n", buffer);
}
