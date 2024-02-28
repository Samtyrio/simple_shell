#include "shell.h"

/**
 * path_resolve - Resolve the full path of a command using the PATH
 * @command: The command to resolve
 * Return: The full path of the command if found, NULL otherwise
 */
char *path_resolve(const char *command)
{
	char *path, *dir, *full_path;

	if (command[0] == '/')
	{
		return (strdup(command));
	}

	path = strdup(getenv("PATH"));
	if (path == NULL)
	{
		perror("strdup");
		return (NULL);
	}

	dir = strtok(path, ":");
	while (dir != NULL)
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (full_path == NULL)
		{
			perror("malloc");
			free(path);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", dir, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path);
	return (NULL);
}
