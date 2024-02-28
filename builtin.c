#include "shell.h"

/**
 * exit_builtin - Exit the shell
 */
void exit_builtin(void)
{
	exit(EXIT_SUCCESS);
}

/**
 * env_builtin - Print the current environment
 */
void env_builtin(void)
{
	char **env = environ;

	while (*env)
	{
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
		env++;
	}
}
