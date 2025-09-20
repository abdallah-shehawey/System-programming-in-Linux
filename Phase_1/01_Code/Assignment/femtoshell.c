#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char line[11000];
  const char *prompt = "FemtoShell> ";
  int last_status = 0;

  while (1)
  {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(line, sizeof(line), stdin) == NULL)
    {
      break;
    }

    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
      line[len - 1] = '\0';
      len--;
    }

    if (len == 0)
    {
      last_status = 0;
      continue;
    }

    if (strcmp(line, "exit") == 0)
    {
      printf("Good Bye\n");
      return 0;
    }

    if (strncmp(line, "echo", 4) == 0 && (line[4] == ' ' || line[4] == '\0'))
    {
      if (line[4] == ' ')
      {
        printf("%s\n", line + 5);
      }
      else
      {
        printf("\n");
      }
      last_status = 0;
      continue;
    }

    printf("Invalid command\n");
    last_status = 1;
  }

  return last_status;
}
