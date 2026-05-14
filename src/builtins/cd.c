#include "../../include/my_shell.h"

// cd, cd [path] "DONE", cd - (previous dir) - cd ~ (home dir), handle non existing dirs, permission issues
int command_cd(char** args, char* initial_directory)
{
    (void)initial_directory;

    char* target_directory;

    if (args[1] == NULL) 
    {
        target_directory = getenv("HOME");
        printf("%s\n", target_directory);

        if (target_directory == NULL) 
        {
            printf("cd: HOME not set\n");
            return 1;
        }
    } 
    else
    {
        target_directory = args[1];

        // Try changing directory
        if (chdir(target_directory) != 0) 
        {
            perror("cd");
            return 1;
        }
    }
    return 0; 
}

