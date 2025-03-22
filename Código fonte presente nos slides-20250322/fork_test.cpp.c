#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int value = 0;

int main(int argc, char* argv[])
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		printf("Fork failed!\n");
		return 1;
	}
	else if (pid == 0)
	{
		value = 1;
		while (true)
		{
			printf("Filho - Pid: %d - Value: %d\n", getpid(), value);
			sleep(1);
		}
	}
	else
	{
		while (true)
		{
			printf("Pai - Pid: %d - Value: %d\n", getpid(), value);
			sleep(1);
		}
	}
	return(0);
}
