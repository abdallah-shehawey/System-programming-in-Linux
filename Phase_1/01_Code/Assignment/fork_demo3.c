#include <stdio.h>
#include <unistd.h>

int x = 5;
int y;

int main(void)
{
  int z = 10;
  getchar();

  x++;
  y++;
  z++;
  printf("Parent: before fork x = %d, y = %d, z = %d \n", x, y, z);
  pid_t pid = fork();

  if (pid > 0)
  {
    while (1)
    {
      printf("PARENT: my pid = %d, My child pid = %d\n", getpid(), pid);
      printf("Parent: after fork x = %d, y = %d, z = %d\n", x, y, z);
      x++;
      y++;
      z++;
      usleep(500000);
    }
  }
  else if (pid == 0)
  {

    while (1)
    {

      printf("CHILD: my pid = %d, My Parent pid = %d\n", getpid(), getppid());
      printf("CHILD: after fork x = %d, y = %d, z = %d\n", x, y, z);
      x++;
      y++;
      z++;
      x++;
      y++;
      z++;
      x++;
      y++;
      z++;
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
