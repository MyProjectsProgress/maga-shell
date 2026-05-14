#include "../include/my_shell.h"

// MAIN FUNCTION
int main(int argc, char** argv, char** env) {

    (void)argc;
    (void)argv;

    // Trying to understand what the arguments are 
    // printf("%d\n",argc);

    // printf("%s\n",argv);

    // if (argv == NULL) { return 1; }

    // int i = 0;

    // while (argv[i] != NULL) {
    //     printf("%s\n", argv[i]);
    //     printf("%s\n", "MEGZAWY");
    //     i++;
    // }

    shell_loop(env);
    
    return 0;
}