#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct var
{
  char *name;
  char *value;
  int exported;
};

static void append_char(char **buf, int *len, int *cap, char c)
{
  if (*len >= *cap)
  {
    *cap = *cap ? *cap * 2 : 10;
    *buf = (char *)realloc(*buf, *cap);
  }
  (*buf)[(*len)++] = c;
}

static void append_str(char **buf, int *len, int *cap, const char *s)
{
  while (*s)
  {
    append_char(buf, len, cap, *s++);
  }
}

static char *expand_variable(const char *arg, struct var *vars, int var_count)
{
  char *result = NULL;
  int res_len = 0;
  int res_cap = 0;
  const char *p = arg;
  while (*p)
  {
    if (*p == '$')
    {
      p++;
      const char *start = p;
      while (*p && (isalnum(*p) || *p == '_'))
        p++;
      int len = p - start;
      if (len == 0)
      {
        append_char(&result, &res_len, &res_cap, '$');
      }
      else
      {
        char name[len + 1];
        strncpy(name, start, len);
        name[len] = 0;
        char *val = NULL;
        for (int i = 0; i < var_count; i++)
        {
          if (strcmp(vars[i].name, name) == 0)
          {
            val = vars[i].value;
            break;
          }
        }
        if (val)
        {
          append_str(&result, &res_len, &res_cap, val);
        }
      }
    }
    else
    {
      append_char(&result, &res_len, &res_cap, *p);
      p++;
    }
  }
  if (res_len >= res_cap)
  {
    res_cap = res_len + 1;
    result = (char *)realloc(result, res_cap);
  }
  result[res_len] = 0;
  return result;
}

static void free_vars(struct var *vars, int count)
{
  for (int i = 0; i < count; i++)
  {
    free(vars[i].name);
    free(vars[i].value);
  }
  free(vars);
}

extern char **environ;

int nanoshell_main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  char line[1024];
  struct var *vars = NULL;
  int var_count = 0;
  int var_capacity = 0;
  int last_status = 0;

  while (1)
  {
    printf("Nano Shell Prompt > ");
    fflush(stdout);
    if (!fgets(line, sizeof(line), stdin))
    {
      break;
    }
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
    {
      line[--len] = 0;
    }
    if (len == 0)
    {
      continue;
    }

    int is_assignment = 0;
    char *var_name = NULL;
    char *var_value = NULL;
    char *eq = strchr(line, '=');
    if (eq)
    {
      is_assignment = 1;
      char *p = line;
      while (p < eq)
      {
        if (*p == ' ')
        {
          is_assignment = -1;
          break;
        }
        p++;
      }
      if (is_assignment != -1)
      {
        p = eq + 1;
        while (*p)
        {
          if (*p == ' ')
          {
            is_assignment = -1;
            break;
          }
          p++;
        }
      }
      if (is_assignment == 1)
      {
        *eq = 0;
        var_name = strdup(line);
        var_value = strdup(eq + 1);
        *eq = '=';
      }
    }

    if (is_assignment == -1)
    {
      printf("Invalid command\n");
      last_status = 1;
      continue;
    }

    if (is_assignment == 1)
    {
      int found = -1;
      for (int i = 0; i < var_count; i++)
      {
        if (strcmp(vars[i].name, var_name) == 0)
        {
          found = i;
          break;
        }
      }
      if (found != -1)
      {
        free(vars[found].value);
        vars[found].value = strdup(var_value);
      }
      else
      {
        if (var_count >= var_capacity)
        {
          var_capacity = var_capacity ? var_capacity * 2 : 10;
          vars = (struct var *)realloc(vars, var_capacity * sizeof(struct var));
        }
        vars[var_count].name = strdup(var_name);
        vars[var_count].value = strdup(var_value);
        vars[var_count].exported = 0;
        var_count++;
      }
      free(var_name);
      free(var_value);
      last_status = 0;
      continue;
    }

    char **args = NULL;
    int arg_count = 0;
    int arg_capacity = 0;
    char *token = strtok(line, " ");
    while (token)
    {
      char *expanded = expand_variable(token, vars, var_count);
      if (arg_count >= arg_capacity)
      {
        arg_capacity = arg_capacity ? arg_capacity * 2 : 10;
        args = (char **)realloc(args, arg_capacity * sizeof(char *));
      }
      args[arg_count++] = expanded;
      token = strtok(NULL, " ");
    }
    if (arg_count == 0)
    {
      continue;
    }
    if (arg_count >= arg_capacity)
    {
      args = (char **)realloc(args, (arg_count + 1) * sizeof(char *));
    }
    args[arg_count] = NULL;

    int cmd_status = 0;

    if (strcmp(args[0], "exit") == 0)
    {
      printf("Good Bye\n");
      for (int i = 0; i < arg_count; i++)
      {
        free(args[i]);
      }
      free(args);
      break;
    }
    else if (strcmp(args[0], "cd") == 0)
    {
      char *path = (arg_count > 1) ? args[1] : getenv("HOME");
      if (chdir(path) != 0)
      {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        cmd_status = 1;
      }
    }
    else if (strcmp(args[0], "pwd") == 0)
    {
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        printf("%s\n", cwd);
      }
      else
      {
        perror("pwd");
        cmd_status = 1;
      }
    }
    else if (strcmp(args[0], "echo") == 0)
    {
      for (int i = 1; i < arg_count; i++)
      {
        printf("%s", args[i]);
        if (i < arg_count - 1)
        {
          printf(" ");
        }
      }
      printf("\n");
    }
    else if (strcmp(args[0], "export") == 0)
    {
      if (arg_count < 2)
      {
        fprintf(stderr, "export: missing argument\n");
        cmd_status = 1;
      }
      else
      {
        int found = -1;
        for (int i = 0; i < var_count; i++)
        {
          if (strcmp(vars[i].name, args[1]) == 0)
          {
            found = i;
            break;
          }
        }
        if (found != -1)
        {
          vars[found].exported = 1;
        }
        else
        {
          fprintf(stderr, "export: %s not found\n", args[1]);
          cmd_status = 1;
        }
      }
    }
    else
    {
      pid_t pid = fork();
      if (pid == -1)
      {
        perror("fork");
        cmd_status = 1;
      }
      else if (pid == 0)
      {
        int env_count = 0;
        while (environ[env_count])
          env_count++;
        int extra = 0;
        for (int i = 0; i < var_count; i++)
        {
          if (vars[i].exported)
            extra++;
        }
        char **new_envp = (char **)malloc((env_count + extra + 1) * sizeof(char *));
        int idx = 0;
        for (int i = 0; i < env_count; i++)
        {
          new_envp[idx++] = strdup(environ[i]);
        }
        for (int i = 0; i < var_count; i++)
        {
          if (vars[i].exported)
          {
            size_t entry_len = strlen(vars[i].name) + strlen(vars[i].value) + 2;
            char *entry = (char *)malloc(entry_len);
            snprintf(entry, entry_len, "%s=%s", vars[i].name, vars[i].value);
            int found = -1;
            for (int j = 0; j < idx; j++)
            {
              size_t name_len = strlen(vars[i].name);
              if (strncmp(new_envp[j], vars[i].name, name_len) == 0 && new_envp[j][name_len] == '=')
              {
                found = j;
                break;
              }
            }
            if (found != -1)
            {
              free(new_envp[found]);
              new_envp[found] = entry;
            }
            else
            {
              new_envp[idx++] = entry;
            }
          }
        }
        new_envp[idx] = NULL;
        environ = new_envp;
        if (execvp(args[0], args) == -1)
        {
          if (errno == ENOENT || errno == ENOTDIR)
          {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(127);
          }
          else
          {
            perror(args[0]);
            exit(1);
          }
        }
        exit(0);
      }
      else
      {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
          cmd_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
          cmd_status = 128 + WTERMSIG(status);
        }
        else
        {
          cmd_status = 1;
        }
      }
    }

    last_status = cmd_status;

    for (int i = 0; i < arg_count; i++)
    {
      free(args[i]);
    }
    free(args);
  }

  free_vars(vars, var_count);
  return last_status;
}