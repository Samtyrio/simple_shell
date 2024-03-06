#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define EXIT_SHELL 2
#define BUFFER_SIZE 1024

/*function prototypes*/
char *readline(void);
int execute_line(char *line);
char *path_resolve(const char *command);
void env_builtin(void);
void exit_builtin(void);
int tokenize_input(char *line, char *args[]);
char *get_full_path(char *command);
void handle_builtin_commands(char *args[]);
int execute_command(char *full_path, char *args[]);
void print_command_not_found_error(char *command);
char *resolve_command(char *command);
void print_environment(void);
#endif
