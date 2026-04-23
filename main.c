#include <stdio.h>
#include <stdlib.h>
#include "my_shell.h"

void shell_loop(char** env){
    char* input = NULL;
    size_t input_size = 0;
    
    char** args;

    while(1) {
        printf("[my_shell]>");
        if (getline(&input, &input_size, stdin) == -1) {
            // end of the file EOF, ctrl+D
            perror("getline");
            break;
        }
        // printf("Input: %s", input);
        args = parse_input(input);
        for (size_t i; args[i]; i++){
            printf("Args: %s", args[i]);
        }
        
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