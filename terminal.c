#include "my_shell.h"


// To handle the history of terminal commands
#define HISTORY_SIZE 100
char* history[HISTORY_SIZE];
int history_count = 0;

// to disable the ICANON and ECHO modes
struct termios original_termios;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

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
    int position = 0;

    int history_index = history_count;

    char* buffer = malloc(buffer_size);

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
                    Erase current line visually
                    */
                    while (position > 0) {
                        write(STDOUT_FILENO, "\b \b", 3);
                        position--;
                    }
                    /*
                    Copy history command into buffer
                    */
                   
                    strcpy(buffer, history[history_index]);

                    position = strlen(buffer);

                    /*
                    Print recalled command
                    */
                    write(STDOUT_FILENO, buffer, position);
                }
                continue;
            }
        }

        /*
        ENTER KEY
        '\n' = newline
        '\r' = carriage return
        terminals may send either
        */
        if (c == '\n' || c == '\r') {
            write(STDOUT_FILENO, "\n", 1); // means: write one byte (a character \n) to terminal
            buffer[position] = '\0';
            return buffer;
        }

       /*
        BACKSPACE handling
        ASCII 127 = delete key
        */
        else if (c == 127) {
            if (position > 0) {
                position--;
                /*
                Move cursor back,
                erase char,
                move cursor back again
                */
                write(STDOUT_FILENO, "\b \b", 3);
            }
        }

        /*
        NORMAL CHARACTER
        */
        else {
            if (position >= buffer_size - 1) {

                buffer_size *= 2;

                char* new_buffer = realloc(buffer, buffer_size);

                if (!new_buffer) {
                    free(buffer);
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }

                buffer = new_buffer;
            }
            // Store character in buffer
            buffer[position] = c;

            position++;

            // Print character manually
            write(STDOUT_FILENO, &c, 1);
        }
    }
}


void add_to_history(char* command)
{
    if (history_count < HISTORY_SIZE){
        history[history_count] = strdup(command); // means: allocate new memory, copy string into it, return pointer
        history_count++;
    }
}