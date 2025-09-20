#include <stdio.h>
#include <unistd.h>

int main(void)
{
  getchar();

  pid_t pid = fork();

  if (pid > 0)
  {
    while (1)
    {
      printf("PARENT: my pid = %d, My child pid = %d\n", getpid(), pid);
      usleep(500000);
    }
  }
  else if (pid == 0)
  {

    while (1)
    {

      printf("CHILD: my pid = %d, My Parent pid = %d\n", getpid(), getppid());
      usleep(500000);
    }
  }
  else
  {
    printf("PARENT: failed to fork\n");
  }

  getchar();
  return 0;
}
