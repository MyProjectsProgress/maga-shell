#include "../../include/my_shell.h"

// Refactored which implementation after using path_utils in which and in external executable to find the path.

int command_which(char** args, char** env)
{
    int i = 1;
    char* path;

    if (!args[1]) { printf("which: missing argument\n"); return 1; }

    while (args[i])
    {
        path = find_executable(args[i], env);
        
        if (path)
        {
            printf("%s\n", path);
            free(path);
        }
        else { printf("%s not found\n", path); }

        i++;
    }

    return 0;
}



// char* get_path_value(char** env)
// {
//     int i = 0;

//     while (env[i])
//     {
//         if (strncmp(env[i], "PATH=", 5) == 0) { return env[i] + 5; } // +5 is here to skip PATH=
//         i++;
//     }

//     return NULL;
// }

// int command_which(char** args, char** env)
// {
//     char* path = get_path_value(env);
//     char* path_copy;
//     char* dir;
//     char full_path[1024]; //empty string container to copy the created path in 
//     int i = 1;

//     if (!args[1]) { printf("which: missing argument\n"); return 1; } // if not NULL is true so it means it is NULL!

//     while (args[i])
//     {
//         path_copy = strdup(path); // Copies PATH string.

//         dir = strtok(path_copy, ":"); // Splits: /usr/bin:/bin:/usr/local/bin into tokens: /usr/bin, /bin, /usr/local/bin

//         while (dir)
//         {
//             snprintf(full_path, sizeof(full_path), "%s/%s", dir, args[i]); // Build full candidate path, ex: /bin + / + ls becomes /bin/ls 

//             if (access(full_path, X_OK) == 0) // Checks: file exists, executable permission exists
//             { 
//                 printf("%s\n", full_path); 
//                 break; 
//             } 

//             dir = strtok(NULL, ":");
//         }

//         if (dir == NULL) { printf("%s not found\n" , args[i]); }

//         free(path_copy);

//         i++;
//     }

//     return 0;
// }