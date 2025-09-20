#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100

int main(void)
{
    char buf[BUF_SIZE];

    while (1) {
	printf("I am a simple shell, enter your cmd $");
	fgets(buf, BUF_SIZE, stdin);
	buf[strlen(buf) - 1] = 0;
	if (strlen(buf) == 0)
	    continue;

	pid_t pid = fork();

	if (pid > 0) {
	    int status;
	    printf("PARENT: my pid = %d, My child pid = %d\n", getpid(),
		   pid);
	    wait(&status);
	    printf("PARENT: my pid = %d, My child status = %d\n", getpid(), WEXITSTATUS(status));

	} else if (pid == 0) {
	    printf("CHILD: my pid = %d, My Parent pid = %d\n", getpid(),
		   getppid());
	    char *newargv[] = { buf, NULL };
	    char *newenvp[] = { NULL };
	    execve(buf, newargv, newenvp);
	    printf("exec failed\n");
	    exit(-1);
	} else {
	    printf("PARENT: failed to fork\n");
	}
    }
    return 0;

}
