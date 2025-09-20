#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//./executer /usr/bin/ls

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s path-to-elf\n", argv[0]);
    exit(-1);
  }

  char *newargv[] = {argv[1], NULL};
  char *newenvp[] = {NULL};
  execve(argv[1], newargv, newenvp);
  printf("exec failed");
  return -2;
}
