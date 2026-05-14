#include "../../include/my_shell.h"

int command_pwd()
{
    char* cwd = NULL;

    // use dynamic allocation
    cwd = getcwd(NULL, 0);

    if (cwd != NULL)
    {
        printf("%s\n", cwd);
        free(cwd);
    } 
    else { perror("getcwd"); }

    return 0;
}