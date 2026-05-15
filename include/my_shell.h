#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include <termios.h>
#include <string.h>

#define MAX_INPUT 128
#define MAX_PATH 256

// 1- Controllers functions:
int shell_router(char** args, char** env, char* initial_directory);
void shell_loop(char** env);


// 2- Built-in functions:
int command_cd(char** args, char* initial_directory); // cd.c
int command_pwd(); // pwd.c
int command_echo(char** args, char** env); // echo.c
int command_exit(); // exit.c
int command_env(char** env); // env.c
int command_which(char** args, char** env); // which.c
// char** command_setenv(char** args, char** env);
// char** command_unsetenv(char** args, char** env);


// 3- Services functions:
char** parse_input(char* input);
void free_tokens(char** tokens);
int execute_external_command(char** args, char** env);
char* find_command_path(char* command, char* env);


// 4- View functions:
void disable_raw_mode();
void enable_raw_mode();
char* read_input();
void add_to_history(char* command);
void clear_line(int* position);


// 5- Utils
// string_utils.c
int my_strcmp(const char* str1, const char* str2);
// path_utils.c
char* find_executable(char* command, char** env);
char* get_path_value(char** env);
