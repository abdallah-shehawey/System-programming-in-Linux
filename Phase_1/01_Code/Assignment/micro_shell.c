#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_VARS 100
#define PROMPT "microshell> "

typedef struct
{
  char *name;
  char *value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

char *find_variable(const char *name)
{
  for (int i = 0; i < var_count; i++)
  {
    if (strcmp(vars[i].name, name) == 0)
    {
      return vars[i].value;
    }
  }
  return NULL;
}

void set_variable(const char *name, const char *value)
{
  for (int i = 0; i < var_count; i++)
  {
    if (strcmp(vars[i].name, name) == 0)
    {
      free(vars[i].value);
      vars[i].value = strdup(value);
      return;
    }
  }
  if (var_count < MAX_VARS)
  {
    vars[var_count].name = strdup(name);
    vars[var_count].value = strdup(value);
    var_count++;
  }
}

void substitute_variables(char *arg, char *result, int max_len)
{
  char *out = result;
  *out = '\0';
  while (*arg && (out - result) < max_len - 1)
  {
    if (*arg == '$')
    {
      arg++;
      char var_name[256] = {0};
      int j = 0;
      while (*arg && (isalnum(*arg) || *arg == '_') && j < 255)
      {
        var_name[j++] = *arg++;
      }
      var_name[j] = '\0';
      char *var_value = find_variable(var_name);
      if (var_value)
      {
        size_t len = strlen(var_value);
        size_t space = max_len - (out - result) - 1;
        if (len > space)
          len = space;
        memcpy(out, var_value, len);
        out += len;
      }
    }
    else
    {
      *out++ = *arg++;
    }
  }
  *out = '\0';
}

// build_args(tokens, token_count, args);
int build_args(char **tokens, int token_count, char **args)
{
  int arg_count = 0;
  for (int i = 0; i < token_count;)
  {
    if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], "2>") == 0)
    {
      char *op = tokens[i];
      i++;
      if (i >= token_count)
      {
        fprintf(stderr, "missing file after '%s'\n", op);
        return -1;
      }
      i++;
    }
    else
    {
      args[arg_count++] = tokens[i];
      i++;
    }
  }
  return arg_count;
}

int apply_redirections(char **tokens, int token_count, int is_builtin)
{
  for (int i = 0; i < token_count;)
  {
    if (strcmp(tokens[i], "<") == 0)
    {
      i++;
      if (i >= token_count)
      {
        fprintf(stderr, "missing file after '<'\n");
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      char res[256];
      substitute_variables(tokens[i], res, sizeof(res));
      int fd = open(res, O_RDONLY);
      if (fd == -1)
      {
        fprintf(stderr, "cannot access %s: No such file or directory\n", res);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      if (dup2(fd, STDIN_FILENO) == -1)
      {
        perror("dup2 input");
        close(fd);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      close(fd);
      i++;
    }
    else if (strcmp(tokens[i], ">") == 0)
    {
      i++;
      if (i >= token_count)
      {
        fprintf(stderr, "missing file after '>'\n");
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      char res[256];
      substitute_variables(tokens[i], res, sizeof(res));
      int fd = open(res, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd == -1)
      {
        fprintf(stderr, "%s: Permission denied\n", res);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      if (dup2(fd, STDOUT_FILENO) == -1)
      {
        perror("dup2 output");
        close(fd);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      close(fd);
      i++;
    }
    else if (strcmp(tokens[i], "2>") == 0)
    {
      i++;
      if (i >= token_count)
      {
        fprintf(stderr, "missing file after '2>'\n");
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      char res[256];
      substitute_variables(tokens[i], res, sizeof(res));
      int fd = open(res, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd == -1)
      {
        fprintf(stderr, "%s: Permission denied\n", res);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      if (dup2(fd, STDERR_FILENO) == -1)
      {
        perror("dup2 error");
        close(fd);
        if (is_builtin)
          return -1;
        else
          exit(1);
      }
      close(fd);
      i++;
    }
    else
    {
      i++;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  char input[MAX_LINE];
  char *tokens[MAX_ARGS];
  int status = 0;

  while (1)
  {
    printf(PROMPT);
    fflush(stdout);

    if (!fgets(input, MAX_LINE, stdin))
    {
      break;
    }

    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) == 0)
    {
      continue;
    }

    int token_count = 0;
    char *token = strtok(input, " \t");
    while (token && token_count < MAX_ARGS - 1)
    {
      tokens[token_count++] = token;
      token = strtok(NULL, " \t");
    }
    tokens[token_count] = NULL;

    if (token_count == 0)
    {
      continue;
    }

    char *args[MAX_ARGS];
    int arg_count = build_args(tokens, token_count, args);
    if (arg_count < 0)
    {
      status = 1;
      continue;
    }

    if (arg_count == 0)
    {
      continue; // no command
    }

    // Check for variable assignment
    char *equal_sign = strchr(args[0], '=');
    if (equal_sign && equal_sign != args[0])
    {
      *equal_sign = '\0';
      char *var_name = args[0];
      char *var_value = equal_sign + 1;
      set_variable(var_name, var_value);
      continue;
    }

    // Handle exit
    if (strcmp(args[0], "exit") == 0)
    {
      printf("Good Bye\n");
      break;
    }

    // Save original fds for built-ins
    int save_in = -1, save_out = -1, save_err = -1;
    int is_builtin = 0;

    if (strcmp(args[0], "echo") == 0 || strcmp(args[0], "pwd") == 0 || strcmp(args[0], "cd") == 0 ||
        strcmp(args[0], "export") == 0)
    {
      is_builtin = 1;
      save_in = dup(STDIN_FILENO);
      save_out = dup(STDOUT_FILENO);
      save_err = dup(STDERR_FILENO);
      if (apply_redirections(tokens, token_count, 1) == -1)
      {
        status = 1;
      }
      else
      {
        // Execute built-in
        if (strcmp(args[0], "echo") == 0)
        {
          for (int j = 1; j < arg_count; j++)
          {
            char result[256];
            substitute_variables(args[j], result, sizeof(result));
            printf("%s", result);
            if (j < arg_count - 1)
              printf(" ");
          }
          printf("\n");
          status = 0;
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
          char cwd[PATH_MAX];
          if (getcwd(cwd, sizeof(cwd)))
          {
            printf("%s\n", cwd);
            status = 0;
          }
          else
          {
            perror("getcwd");
            status = 1;
          }
        }
        else if (strcmp(args[0], "cd") == 0)
        {
          char *dir = arg_count > 1 ? args[1] : getenv("HOME");
          char resolved_dir[256];
          substitute_variables(dir, resolved_dir, sizeof(resolved_dir));
          if (chdir(resolved_dir) != 0)
          {
            fprintf(stderr, "cd: %s: No such file or directory\n", resolved_dir);
            status = 1;
          }
          else
          {
            status = 0;
          }
        }
        else if (strcmp(args[0], "export") == 0)
        {
          if (arg_count > 1)
          {
            char *var_name = args[1];
            char *var_value = find_variable(var_name);
            if (var_value)
            {
              setenv(var_name, var_value, 1);
            }
          }
          status = 0;
        }
      }
      dup2(save_in, STDIN_FILENO);
      close(save_in);
      dup2(save_out, STDOUT_FILENO);
      close(save_out);
      dup2(save_err, STDERR_FILENO);
      close(save_err);
      continue;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
      perror("fork");
      status = 1;
      continue;
    }

    if (pid == 0)
    {
      apply_redirections(tokens, token_count, 0);

      char *resolved_args[MAX_ARGS];
      for (int j = 0; j < arg_count; j++)
      {
        resolved_args[j] = (char *)malloc(256);
        substitute_variables(args[j], resolved_args[j], 256);
      }
      resolved_args[arg_count] = NULL;

      execvp(resolved_args[0], resolved_args);
      fprintf(stderr, "%s: command not found\n", resolved_args[0]);
      for (int j = 0; j < arg_count; j++)
      {
        free(resolved_args[j]);
      }
      exit(1);
    }
    else
    {
      int child_status;
      waitpid(pid, &child_status, 0);
      status = WEXITSTATUS(child_status);
    }
  }

  for (int i = 0; i < var_count; i++)
  {
    free(vars[i].name);
    free(vars[i].value);
  }

  return status;
}
