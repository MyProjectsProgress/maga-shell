#include "my_shell.h"

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

int command_pwd()
{
    char* cwd = NULL;

    // use dynamic allocation
    cwd = getcwd (NULL, 0);
    if (cwd != NULL){
        printf("%s\n", cwd);
        free(cwd);
    } else {
        perror("getcwd");
    }
    return 0;
}

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

int command_env(char** env);
int command_which(char** args, char** env);

char** command_setenv(char** args, char** env);
char** command_unsetenv(char** args, char** env);

