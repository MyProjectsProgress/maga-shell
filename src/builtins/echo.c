#include "../../include/my_shell.h"

// echo Hello World, echo -n Hello, echo &PATH
int command_echo(char** args, char** env)
{
    int new_line_flag = 1; // Default echo ends with newline
    int  i = 1; // Skipping the -n

    if (args[1] != NULL && my_strcmp(args[1], "-n") == 0){
        new_line_flag = 0;
        i++;
    }

    // Processing remaining args
    for (; args[i];i++){
        if (args[i][0] == '$'){ // Handle environment variables
            (void)env;
        } else {
            printf("%s", args[i]);
        }

        if (args[i+1] != NULL){
            printf(" ");
        }
    }

    if (new_line_flag){
        printf("\n");
    }

    return 0;


}