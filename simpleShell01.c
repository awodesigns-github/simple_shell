#include "main.h"

#define MAX_INPUT 256

/**
 * _getline - gets line from the stdin
 *
 * Return: string input from buffer
 */
char* _getline(void)
{
        char *buffer;
        size_t buffSize = MAX_INPUT;
        int charCount;

        buffer = (char *)malloc(buffSize * sizeof(char));
        if (buffer == NULL)
        {
                perror("Failed to assign buffer ...");
                exit(EXIT_FAILURE);
        }
        printf("#cisfun$ ");
        charCount = getline(&buffer, &buffSize, stdin);
        if (charCount == -1)
        {
                if (feof(stdin))
                {
                        kill(getpid(), SIGINT);
                }
                perror("Failed to get input...");
                exit(EXIT_FAILURE);
        }
        return (buffer);
}

/**
 * _tokenize - tokenize the input buffer string
 *
 * @str: string to tokenize
 *
 * Return: array of strings
 */
char** _tokenize(char* str)
{
        char** argv;
        char* token;
        int i = 0;

        argv = malloc(MAX_INPUT * sizeof(char*));
        if (argv == NULL)
        {
                perror("Failed to allocate memory...");
                exit(EXIT_FAILURE);
        }
        token = strtok(str, " \n");
        while (token != NULL)
        {
                argv[i++] = token;
                token = strtok(NULL, " \n");
        }
        argv[i] = NULL;
        return (argv);
}

/**
* main - implementation of simple shell one
*
* Return: Always 0
*/
int main(void)
{
        pid_t pid;
        char *cmd;
        int status;
        char **argv;

        pid = fork();
        while (1)
        {
                if (pid == 0)
                {
			cmd = _getline();
			argv = _tokenize(cmd);
			if (execve(argv[0], argv, NULL) == -1)
			{
				perror("simple_shell");
				exit(EXIT_FAILURE);
			}
                } else if (pid > 0)
		{
			wait(&status);
			pid = fork();
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			{
				break;
			}
		}
        }
        return (0);
}
