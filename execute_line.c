#include "shell.h"

/* Function prototypes */
int process_input(char *line);
int tokenize_input(char *line, char *args[]);
char *resolve_command(char *command);
int execute_command(char *full_path, char *args[]);
void cleanup_execution(char *full_path, pid_t pid);

/**
 * execute_line - Executes the line itself
 * @line: The function that takes care of the line
 * Return: What is needed
 */
int execute_line(char *line)
{
	if (*line == '\n')
		return (0);

	if (strcmp(line, "exit\n") == 0)
		return (EXIT_SHELL);

	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';

	return (process_input(line));
}

/**
 * process_input - Inputs the process
 * @line: The parameter
 * Return: What is needed
 */
int process_input(char *line)
{
	char *args[BUFFER_SIZE], *full_path;
	int i, result;
	pid_t pid;

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
		result = execute_command(full_path, args);
		free(full_path);
		_exit(result == -1 ? 1 : 0);
	}
	else
	{
		cleanup_execution(full_path, pid);
	}

	return (0);
}

/**
 * cleanup_execution - Cleanup the execution
 * @full_path: Handle the path
 * @pid: Handle the pid
 * Return: What is needed
 */
void cleanup_execution(char *full_path, pid_t pid)
{
	int status;

	free(full_path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
		return;
	else
		return;
}
