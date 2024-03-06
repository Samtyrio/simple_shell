#include "shell.h"
#include <unistd.h>

/**
 * tokenize_input - Tokenize the input line into arguments
 * @line: The input line containing the command
 * @args: Array to store the arguments
 * Return: Number of arguments or -1 on failure
 */
int tokenize_input(char *line, char *args[])
{
	char *token;
	int i;

	token = strtok(line, " ");
	i = 0;
	while (token != NULL && i < BUFFER_SIZE - 1)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	if (i == 0)
	{
		write(STDERR_FILENO, "No command entered\n", 19);
		return (-1);
	}
	return (i);
}

/**
 * resolve_command - Resolve the full path of the command
 * @command: The command to resolve
 * Return: The full path of the command if found, NULL otherwise
 */
char *resolve_command(char *command)
{
	char *full_path, error_msg[1024];

	if (command[0] == '/')
	{
		return (strdup(command));
	}
	else if (strcmp(command, "env") == 0)
	{
		print_environment();
		return (NULL);
	}
	else
	{
		full_path = path_resolve(command);
		if (full_path == NULL)
		{
			snprintf(error_msg, sizeof(error_msg), "%s: command not found\n", command);
			write(STDERR_FILENO, error_msg, strlen(error_msg));
		}
		return (full_path);
	}
}

/**
 * execute_command - Execute the command with given full path and arguments
 * @full_path: The full path of the command
 * @args: Array of arguments for the command
 * Return: 0 on success, -1 on failure
 */
int execute_command(char *full_path, char *args[])
{
	if (execve(full_path, args, NULL) == -1)
	{
		perror("execve");
		return (-1);
	}
	return (0);
}

/**
 * print_environment - Print the current environment
 */
void print_environment(void)
{
	extern char **environ;
	char **env = environ;

	while (*env != NULL)
	{
		size_t len = strlen(*env);
		if (write(STDOUT_FILENO, *env, len) != (ssize_t)len || write(STDOUT_FILENO, "\n", 1) != 1)
		{
			perror("write");
			return;
		}
		env++;
	}
}
