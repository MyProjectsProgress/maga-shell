#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define MAX_INPUT 128;

char** parse_input(char* input);

void free_tokens(char** tokens);

int shell_build(char** args, char** env, char* initial_directory);

// Built-in function implementations
int command_cd(char** args, char* initial_directory);
int command_pwd();
int command_echo(char** args, char** env);
int command_env(char** env);
int command_which(char** args, char** env);

char** command_setenv(char** args, char** env);
char** command_unsetenv(char** args, char** env);