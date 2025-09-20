#include <stdio.h>
#include <unistd.h>

int main(void)
{
  getchar();

  pid_t pid = fork();

  if (pid > 0)
  {
    printf("PARENT: my pid = %d, My child pid = %d\n", getpid(), pid);
  }
  else if (pid == 0)
  {
    printf("CHILD: my pid = %d, My Parent pid = %d\n", getpid(), getppid());
  }
  else
  {
    printf("PARENT: failed to fork\n");
  }

  getchar();
  return 0;
}
