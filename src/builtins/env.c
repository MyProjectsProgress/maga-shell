#include "../../include/my_shell.h"

int command_env(char** env) 
{
    int i = 0;

    if (env == NULL) { return 1; }

    while (env[i] != NULL)
    {
        printf("%s\n", env[i]);
        i++;
    }

    return 0;
}