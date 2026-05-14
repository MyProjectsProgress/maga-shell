#include <stdio.h>
#include <stdlib.h>
#include "../include/my_shell.h"

// built-ins: echo, cd, pwd, echo, env, setenv, which, exit
// binary commands: ls, cat, .. .
int shell_build(char** args, char** env, char* initial_directory){

    if (my_strcmp(args[0], "cd") == 0){
        return command_cd(args, initial_directory);
    
    } else if (my_strcmp(args[0], "pwd") == 0) {
        return command_pwd(); 

    } else if (my_strcmp(args[0], "echo") == 0) {
        return command_echo(args, env);

    } else if (my_strcmp(args[0], "env") == 0) {
        // return command_env(env);
        return 0;
        
    } else if (my_strcmp(args[0], "which") == 0) {
        // return command_which(args, env);
        return 0;
        
    } else if (my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0) {
        disable_raw_mode();
        exit(EXIT_SUCCESS);
        
    } else {
        // Not a built-in command
        // printf("Command not found: %s\n", args[0]);
        // executer();
    }
    return 0;
}   


// the main shell loop
void shell_loop(char** env){

    char* input = NULL;
    char** args;
    char* initial_directory = getcwd(NULL, 0); // gets current directory.

    enable_raw_mode(); // Enable raw mode in shell

    // Shell Loop (infinite loop)
    while(1) {

        // Always printing the current directory in the begining 
        char* cwd = getcwd(NULL, 0);

        if (cwd != NULL) {
            printf("[%s]>", cwd);
            fflush(stdout);
            free(cwd);
        } 

        input = read_input();

        if (input[0] != '\0') {
            add_to_history(input);
        }

        args = parse_input(input);

        if (args[0]){
            shell_build(args, env, initial_directory); 
        }

        free_tokens(args);
        free(input); // when we free the input from memory, we don't lose it as we saved a copy of it and have its pointer in command_history ;)
        
        // For debugging the input up arrow. 
        // char c;
        // read(STDIN_FILENO, &c, 1);
        // printf("ASCII: %d\n", c);    
    }

    
}

// MAIN FUNCTION
int main(int argc, char** argv, char** env) {

    (void)argc;
    (void)argv;

    shell_loop(env);
    
    return 0;
}