#include "my_shell.h"

char** parse_input(char* input){
    size_t buffer_size = MAX_INPUT;
    char** tokens = malloc(buffer_size * sizeof(char*)); // we first allocate the whole buffer as array of pointers, no actual strings are stored.
    char* token = NULL;
    size_t token_length = 0;
    size_t position = 0;


    if(!tokens){
        perror("Malloc");
        exit(EXIT_FAILURE);
    }

    // we loop o each pointer, then we retrive the string, loop on it char by char, then increase the size of storage for that string based on its characters.
    for (size_t i = 0; input[i]; i++){
        token = &input[i];
        while (input[i] && input[i] != ' '){
            token_length++;
            i++;
        }
        tokens[position] = malloc((token_length+1) * sizeof(char)); //+1 is for the null terminator '\0'
        if(!tokens[position]){
            perror("Malloc");
            exit(EXIT_FAILURE);
        }
        for (size_t j = 0; j <token_length; j++){
            tokens[position][j] = token[j];
        }
        tokens[position][token_length] = '\0'; // Null terminate token
        position ++;
        token_length = 0;
    }

    tokens[position] = NULL;

    return tokens;

}