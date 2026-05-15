#include "../../include/my_shell.h"

char* get_path_value(char** env)
{
    int i = 0;

    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0) { return env[i] + 5; } // +5 is here to skip PATH=
        i++;
    }

    return NULL;
}

char* find_executable(char* command, char** env)
{
    char* path = get_path_value(env);
    char* path_copy;
    char* dir;
    char full_path[1024];

    if (!path) { return NULL; }

    path_copy = strdup(path);
    dir = strtok(path_copy, ":"); 

    while (dir) 
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return strdup(full_path); // return a heap copy because full_path refers to a variable inside a function.
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
