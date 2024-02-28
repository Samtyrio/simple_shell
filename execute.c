#include "shell.h"

/**
 * execute_line - Execute the command entered by the user
 * @line: line Pointer to the input line containing the command
 * Return: 0 on success, 1 on failure, and 2 if the shell should exit
 */
int execute_line(char *line)
{
	char *args[BUFFER_SIZE];
	char *full_path;
	int i, status;
	pid_t pid;

	if (*line == '\n')
		return (0);
	if (strcmp(line, "exit\n") == 0)
		return (EXIT_SHELL);
	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';

	i = tokenize_input(line, args);
	if (i == -1)
		return (1);

	full_path = resolve_command(args[0]);
	if (full_path == NULL)
		return (1);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_path);
		return (1);
	}
	if (pid == 0)
	{
		if (execute_command(full_path, args) == -1)
		{
			free(full_path);
			return (1);
		}
		_exit(EXIT_SUCCESS);
	}
	else
	{
		free(full_path);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
			return (0);
		else
			return (1);
	}
}

/**
 * tokenize_input - Tokenize the input line into arguments
 * @line: line The input line containing the command
 * @args: args Array to store the arguments
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
 * @command: command The command to resolve
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
 * @full_path: full_path The full path of the command
 * @args: args Array of arguments for the command
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
	size_t len;

	while (*env != NULL)
	{
		len = strlen(*env);
		if ((ssize_t)write(STDOUT_FILENO, *env, len) != (ssize_t)len)
		{
			perror("write");
			return;
		}
		if (write(STDOUT_FILENO, "\n", 1) != 1)
		{
			perror("write");
			return;
		}
		env++;
	}
}
