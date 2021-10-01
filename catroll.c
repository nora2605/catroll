#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define chunk 1024

//joins an array of strings to a single one using a delimeter string
char *join(char *args[], char *inter) {
    int i, len;
    char* result;
    len = 0;
    for (i = 0; args[i]; i++)
        len += strlen(args[i]) + 1;
    result = malloc(len);
    result[0] = '\0';
    for (i=0; args[i]; i++) {
        result = strcat(result, args[i]);
        result = strcat(result, inter);
    }
    return result;
}

int main(int argc, char *argv[])
{
    enum { TEXT_MODE, FILE_MODE } input_mode = FILE_MODE;
    size_t optind;
    char *help_text = "No help available yet.";

    for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++)
    {
        switch (argv[optind][1])
        {
            case 't': input_mode = TEXT_MODE; break;
            case 'f': input_mode = FILE_MODE; break;
            case 'h':
                printf(help_text);
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Usage: %s [-tf] <input>\nFor more information see %s -h\n", argv[0], argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (!*(argv + optind))
    {
        fprintf(stderr, "Usage: %s [-tf] <input>\nFor more information see %s -h\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    argv += optind;

    char *input;
    if (input_mode == FILE_MODE)
    {
        FILE *file;
        char *buffer = 0;
        size_t length;

        file = fopen(argv[0], "r");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            fseek(file, 0, SEEK_SET);
            buffer = malloc(length);
            if (buffer)
                fread(buffer, 1, length, file);
            fclose(file);
        }
        else 
        {
            fprintf(stderr, "File \"%s\" not found or inaccessible\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        if (buffer)
            input = buffer;
        else
            exit(EXIT_FAILURE);
    }
    else
        input = join(argv, " ");

    
}