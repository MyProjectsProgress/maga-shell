#include "../../include/my_shell.h"


// To handle the history of terminal commands, Global variables used in more than one function
#define HISTORY_SIZE 100
char* history[HISTORY_SIZE];
int history_count = 0;

// to disable the ICANON and ECHO modes
struct termios original_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

// to enable raw mode, and being able to implement functions that reads user input and output them so that we handle the up arrow.
void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &original_termios); // "save current terminal configuration"

    struct termios raw = original_termios; // contains local terminal flags: canonical mode, echo, signal handling

    raw.c_lflag &= ~(ICANON | ECHO); // turn OFF: ICANON - ECHO, After disabling ICANON: characters arrive instantly, after disabling ECHO: we must print characters manually

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // Applies the new settings.
}

// READER FUNCTION
char* read_input()
{
    int buffer_size = 1024;
    // int position = 0;
    int string_length = 0;
    int cursor = 0; 

    int history_index = history_count;

    char* buffer = malloc(buffer_size);
    buffer[0] = '\0'; // otherwise buffer would be garbage

    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while(1) {
        char c;

        // handles the possible failure of read
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            free(buffer);
            return NULL;
        }

        // Handles UP ARROW
        if (c == 27) {

            char seq[2];

            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            /*
            UP ARROW = ESC [ A
            */
            if (seq[0] == '[' && seq[1] == 'A') {

                // history logic goes here
                if (history_count > 0 && history_index > 0) {

                    history_index--;
                    /*
                    Copy history command into buffer
                    */
                    strcpy(buffer, history[history_index]);
                    string_length = strlen(buffer);
                    cursor = string_length;
                    refresh_line(buffer, string_length, cursor);
                } 

                continue;
            }
            /*
            DOWN ARROW = ESC [ B
            */
            else if (seq[0] == '[' && seq[1] == 'B') {

                // history logic goes here
                if (history_index < history_count - 1 ) {

                    history_index++;
                    /*
                    Copy history command into buffer
                    */
                    strcpy(buffer, history[history_index]);
                    string_length = strlen(buffer);
                    cursor = string_length;
                    refresh_line(buffer, string_length, cursor);
                } 

                // handle printing blank new command line when down arrow is showing no commands
                else if (history_index == history_count - 1) {

                    history_index++;
                    buffer[0] = '\0'; // to evade line looks empty but pressing Enter executes old hidden command
                    string_length = 0;
                    cursor = 0;
                    refresh_line(buffer, string_length, cursor);
                }

                continue;
                
            }
            /*
            RIGHT | LEFT ARROW = ESC [ C, D
            */
            else if (seq[0] == '[' && seq[1] == 'C') {

                if (cursor < string_length) {
                    cursor++;
                    write(STDOUT_FILENO, "\033[C", 3);
                }

                continue;
            }
            else if (seq[0] == '[' && seq[1] == 'D') {
                if (cursor > 0) 
                {
                    cursor--;
                    write(STDOUT_FILENO, "\033[D", 3);
                }

                continue;
            }
            continue;
        }

        /*
        ENTER KEY
        '\n' = newline
        '\r' = carriage return
        terminals may send either
        */
        if (c == '\n' || c == '\r') {
            write(STDOUT_FILENO, "\n", 1); // means: write one byte (a character \n) to terminal
            buffer[string_length] = '\0';
            return buffer;
        }

       /*
        BACKSPACE handling
        ASCII 127 = delete key
        */
        else if (c == 127) 
        {
            if (cursor > 0) 
            {
                /*
                memmove(destination, source, bytes); copy bytes from source → destination safely (even if overlapping)
                Unlike memcpy, memmove handles overlapping memory.
                Important here because source and destination are inside same buffer
                */
                memmove(&buffer[cursor-1], &buffer[cursor], string_length - cursor + 1);
                cursor--;
                string_length--;
                refresh_line(buffer, string_length, cursor);
                // write(STDOUT_FILENO, "\b \b", 3);  /* Move cursor back, erase char, move cursor back again */
            }
        }

        /*
        NORMAL CHARACTER
        */
        else 
        {
            if (string_length >= buffer_size - 1) {

                buffer_size *= 2;

                char* new_buffer = realloc(buffer, buffer_size);

                if (!new_buffer) {
                    free(buffer);
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                buffer = new_buffer;
            }
            memmove( &buffer[cursor + 1], &buffer[cursor], string_length - cursor);

            buffer[cursor] = c;
            cursor++;
            string_length++;

            refresh_line(buffer, string_length, cursor);
        }
    }
}

void refresh_line(char* buffer, int string_length, int cursor)
{
    /*
    \r = carriage return. It moves cursor to column 0.
    Example before:
    [dir]>hello world
                ^
    After:
    ^
    [dir]>hello world
    Cursor is now at line start.
    */
    write(STDOUT_FILENO, "\r", 1); // go to line start

    /*
    redraw prompt: Ex: [/Users/lumionasr/Documents/GitHub/maga-shell]> 
    notice cwd leaks memory, that's why we free it after.
    */ 
    char* cwd = getcwd(NULL, 0);

    char prompt[1024];
    snprintf(prompt, sizeof(prompt), "[%s]>", cwd);
    write(STDOUT_FILENO, prompt, strlen(prompt));
    
    free(cwd);
    // printf("[%s]>", getpwd(NULL, 0)); 

    // redraw buffer (current command) Ex: [/Users/lumionasr/Documents/GitHub/maga-shell]>echo hello world 
    write(STDOUT_FILENO, buffer, string_length); 

    write(STDOUT_FILENO, "\033[K", 3); // clear leftovers, This is ANSI escape code that wipes everything after cursor.

    for (int i = string_length; i > cursor; i--) { write(STDOUT_FILENO, "\033[D", 3); }

    fflush(stdout);
}

void add_to_history(char* command)
{
    if (history_count < HISTORY_SIZE){
        history[history_count] = strdup(command); // means: allocate new memory, copy string into it, return pointer
        history_count++;
    }
}

// replaced with refresh_line()
// void clear_line(int* position)
// {
//     while (*position > 0) {
//         write(STDOUT_FILENO, "\b \b", 3); // \b: move cursor back, " ": overwrite old character with space, \b: move cursor back again
//         (*position)--;
//     }
// }