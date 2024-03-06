#include "shell.h"

/**
 * readline - Read a line of input from the user
 * Return: Pointer to the input line
 */
char *readline(void)
{
	char *line = NULL;
	size_t bufsize = 0;

	write(STDOUT_FILENO, "$ ", 2);

	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			free(line);
			write(STDOUT_FILENO, "\n", 1);
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}
	return (line);
}

/**
 * main - Contains the main function
 * Return: 0 on success
 */
int main(void)
{
	char *line;
	int status;

	while (1)
	{
		line = readline();
		if (line == NULL)
			break;

		status = execute_line(line);
		free(line);

		if (status == EXIT_SHELL)
			break;
	}
	return (0);
}
