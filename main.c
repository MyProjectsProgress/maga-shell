#include <stdio.h>
#include <stdlib.h>
#include "my_shell.h"

// echo, cd, pwd, echo, env, setenv, which, exit
int shell_build(char** args, char** env, char* initial_directory){

    if (my_strcmp(args[0], "cd") == 0){
        // return command_cd(args, initial_directory);
        return 0;
    
    } else if (my_strcmp(args[0], "pwd") == 0) {
        // return command_pwd(); 
        return 0;

    } else if (my_strcmp(args[0], "echo") == 0) {
        // return command_echo(args, env);
        return 0;
        
    } else if (my_strcmp(args[0], "env") == 0) {
        // return command_env(env);
        return 0;
        
    } else if (my_strcmp(args[0], "which") == 0) {
        // return command_which(args, env);
        return 0;
        
    } else if (my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
        
    } else {
        // Not a built-in command
        // printf("Command not found: %s\n", args[0]);
    }
    return 0;
}   

void shell_loop(char** env){
    char* input = NULL;
    size_t input_size = 0;
    
    char** args;

    char* initial_directory = getcwd(NULL, 0); // gets current directory.

    // printf("Current Directory IS: %s", initial_directory);

    while(1) {
        printf("[my_shell]>");
        if (getline(&input, &input_size, stdin) == -1) {
            // end of the file EOF, ctrl+D
            perror("getline");
            break;
        }

        // printf("Input: %s", input);

        args = parse_input(input);

        // for (size_t i = 0 ; args[i]; i++){
        //     printf("Args: %s", args[i]);
        //     printf("\n");
        // }
        
        if (args[0]){
            shell_build(args, env, initial_directory); 
        }

        free_tokens(args);
    }
}

// You use char ** when you have multiple strings such as a pointer to an array of pointers
// char *args[] = {"ls", "-l", NULL};
// // char **ptr = args;
// char      → one character        ('a')
// char *    → string               ("hello")
// char **   → list of strings      (["ls", "-l"])
int main(int argc, char** argv, char** env) {
    (void)argc;
    (void)argv;

    shell_loop(env);
    
    return 0;
}