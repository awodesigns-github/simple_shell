#include "main.h"

/**
 * tokenize - tokenizes the string input command
 *
 * @command: the command itself
 * @delim: string of delimiters
 *
 * Return: ptr to ptr to tokenized strings
 */
char **tokenize(char *command, char *delim)
{
	char *str = strdup(command);
	char *str2 = strdup(command);
	char **argv;
	char *token;
	int token_count = 0, i = 0;

	token = strtok(str, delim);

	/* count the tokens */
	while (token != NULL)
	{
		token_count++;
		token = strtok(NULL, delim);
	}

	/* cpy tokens to array list */
	token = strtok(str2, delim);
	argv = malloc((token_count + 1) * sizeof(char *));
	while (token != NULL)
	{
		argv[i] = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(argv[i], token);
		token = strtok(NULL, delim);
		i++;
	}
	argv[i] = NULL;
	free(str);
	free(str2);
	return (argv);
}

/**
 * main - simple shell 1.0
 *
 * @ac: number of arguments passed to main
 * @av: argument list
 *
 * Return: integer
 */
int main(int ac, char *av[])
{
	pid_t child_pid;
	char *buffer = NULL;
	size_t len = 0;
	ssize_t read;
	char **argv;

	while (1)
	{
		printf("#cisfun$ ");
		read = getline(&buffer, &len, stdin);

		/* handle end-of-file */
		if (read == -1)
		{
			free(buffer);
			printf("Exiting shell ...");
			exit(0);
		}

		/* tokenize the input */
		argv = tokenize(buffer, " \n\t");
		child_pid = fork();
		if (child_pid == 0)
		{
			if (execve(argv[0], argv, NULL) == -1)
			{
				for (ac = 0; ac < 1; ac++)
					perror(av[ac]);
			}
		} else
		{
			wait(NULL);
		}
	}
	free(buffer);
	return (0);
}
