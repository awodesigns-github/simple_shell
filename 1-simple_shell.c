#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

extern char **environ;

/**
 * execute_command - executes the command
 *
 * @command: the command itself
 */
void execute_command(char *command)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork Error!\n");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		char *args[] = {command, NULL};

		if (execve(command, args, environ) == -1)
		{
			perror("exec error");
			exit(EXIT_FAILURE);
		}
	} else
	{
		wait(NULL);
	}
}

/**
 * main - simple shell 1
 *
 * Return: integer
 */
int main(void)
{
	char *buffer = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		printf("$ ");
		read = getline(&buffer, &len, stdin);
		if (read != -1)
		{
			printf("%s\n", buffer);
			execute_command(buffer);
		} else
		{
			printf("Error");
		}
	}
}
