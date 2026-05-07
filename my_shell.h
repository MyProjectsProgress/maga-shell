#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include <termios.h>
#include <string.h>


#define MAX_INPUT 128
#define MAX_PATH 256

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

int my_strcmp(const char* str1, const char* str2);

void disable_raw_mode();
void enable_raw_mode();
char* read_input();
void add_to_history(char* command);
void clear_line(int* position);