#include "main.h"

extern char **environ;

/**
 * find_value_by_key - tokenizes the PATH variable
 *
 * @key: environment variable
 *
 * Return: string
 */
char *find_value_by_key(const char *key)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		/* separate the keys from values */
		char *equal_sign = strchr(environ[i], '=');

		if (equal_sign != NULL)
		{
			size_t key_length = equal_sign - environ[i];

			/* compare the keys */
			if (strncmp(environ[i], key, key_length) == 0)
			{
				return (equal_sign);
			}
		}
	}
	return (NULL);
}

/**
 * tokenize_values - tokenizes the values
 *
 * @str: values
 * @delim: delimiter
 * @max_token: max number
 *
 * Return: array of tokens
 */
char **tokenize_values(char *str, char *delim, int max_token)
{
	char *str_cpy = strdup(str);
	char **tokens;
	char *token;
	int token_count = 0;

	if (str_cpy == NULL)
	{
		perror("duplication error");
		free(str_cpy);
		return (NULL);
	}

	tokens = malloc((max_token + 1) * sizeof(char *));
	if (tokens == NULL)
	{
		free(str_cpy);
		perror("malloc");
		return (NULL);
	}
	token = strtok(str_cpy, delim);
	while (token != NULL)
	{
		tokens[token_count] = strdup(token);
		if (tokens[token_count] == NULL)
		{
			perror("strdup");
			free(tokens);
			free(str_cpy);
			return (NULL);
		}
		token_count++;
		if (token_count >= max_token)
		{
			printf("max number of tokens");
			break;
		}
		token = strtok(NULL, delim);
	}
	tokens[token_count] = NULL;
	free(str_cpy);
	return (tokens);
}

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
	const char *key = "PATH";
	char *value;
	char **values;

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

		/* finding the key */
		value = find_value_by_key(key);
		if (value != NULL)
		{
			printf("Value of %s: %s\n", key, value);
		} else
		{
			printf("%s not found in environment", key);
		}

		/* tokenize the value */
		values = tokenize_values(value, ":", 20);

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
			printf("%s\n", values[0]);
		} else
		{
			wait(NULL);
		}
	}
	free(buffer);
	return (0);
}
