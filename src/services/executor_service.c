#include "../../include/my_shell.h"

int execute_external_command(char** args, char** env)
{
    pid_t pid;
    int status;
    char* executable_path;

    executable_path = find_executable(args[0], env);

    if (!executable_path) { printf("%s: command not found\n", args[0]); return 1; }

    pid = fork();

    if (pid == 0) // child
    {
        execve(executable_path, args, env);
        perror("execve");
        exit(1);
    }

    else if (pid > 0) { waitpid(pid, &status, 0); } // parent

    else { perror("fork"); }

    free(executable_path);

    return 0;

}