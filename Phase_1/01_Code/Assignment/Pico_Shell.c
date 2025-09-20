#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROMPT "Pico shell prompt > "

static char **parse_input(char *line, int *argc_out)
{
  int argc = 0;
  int capacity = 10;
  char **argv = (char **)malloc(capacity * sizeof(char *));
  if (!argv)
  {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  char *token = strtok(line, " \t\n");
  while (token)
  {
    if (argc >= capacity)
    {
      capacity *= 2;
      argv = (char **)realloc(argv, capacity * sizeof(char *));
      if (!argv)
      {
        perror("realloc");
        exit(EXIT_FAILURE);
      }
    }
    argv[argc++] = strdup(token);
    token = strtok(NULL, " \t\n");
  }
  argv[argc] = NULL;
  *argc_out = argc;
  return argv;
}

static void free_args(char **argv, int argc)
{
  for (int i = 0; i < argc; i++)
  {
    free(argv[i]);
  }
  free(argv);
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  int last_status = 0;

  while (1)
  {
    printf("%s", PROMPT);
    fflush(stdout);

    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
      break;
    }

    int cmd_argc;
    char **cmd_argv = parse_input(line, &cmd_argc);
    if (cmd_argc == 0)
    {
      free_args(cmd_argv, cmd_argc);
      continue;
    }

    if (strcmp(cmd_argv[0], "exit") == 0)
    {
      printf("Good Bye\n");
      free_args(cmd_argv, cmd_argc);
      break;
    }
    else if (strcmp(cmd_argv[0], "echo") == 0)
    {
      for (int i = 1; i < cmd_argc; i++)
      {
        printf("%s", cmd_argv[i]);
        if (i < cmd_argc - 1)
          printf(" ");
      }
      printf("\n");
      last_status = 0;
    }
    else if (strcmp(cmd_argv[0], "pwd") == 0)
    {
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        printf("%s\n", cwd);
        last_status = 0;
      }
      else
      {
        perror("pwd");
        last_status = 1;
      }
    }
    else if (strcmp(cmd_argv[0], "cd") == 0)
    {
      if (cmd_argc < 2)
      {
        fprintf(stderr, "cd: missing argument\n");
        last_status = 1;
      }
      else if (chdir(cmd_argv[1]) != 0)
      {
        fprintf(stderr, "cd: %s: %s\n", cmd_argv[1], strerror(errno));
        last_status = 1;
      }
      else
      {
        last_status = 0;
      }
    }
    else
    {
      pid_t pid = fork();
      if (pid == 0)
      {
        execvp(cmd_argv[0], cmd_argv);
        fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
        exit(127);
      }
      else if (pid > 0)
      {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
          last_status = WEXITSTATUS(status);
        }
        else
        {
          last_status = 1;
        }
      }
      else
      {
        perror("fork");
        last_status = 1;
      }
    }

    free_args(cmd_argv, cmd_argc);
  }

  free(line);
  return last_status;
}
