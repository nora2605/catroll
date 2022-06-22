#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>	// for Sleep()
#else 
#include <unistd.h>	// for usleep()
#endif

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

char *cycle(char *str, size_t s) {
    char *res = malloc(s+1);
    for (int i = 0; i < s; i++)
        res[i] = str[(i + 1) % s];
    res[s] = '\0';
    return res;
}

int main(int argc, char *argv[])
{
    enum { TEXT_MODE, FILE_MODE } input_mode = FILE_MODE;
    size_t optind;
    long delay = 100000;
    char *help_text = "No help available yet.";

    for (optind = 1; optind < argc && argv[optind][0] == '-'; optind++)
    {
        switch (argv[optind][1])
        {
            case 't': input_mode = TEXT_MODE; break;
            case 'f': input_mode = FILE_MODE; break;
            case 'd': 
                delay = strtol(argv[++optind], NULL, 10);
                if (!delay) {
                    fprintf(stderr, "Invalid integer for -d.\n");
                    exit(EXIT_FAILURE);
                } else delay *= 1000;
                break;
            case 'h':
                printf(help_text);
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Usage: %s [-tfdh] <input>\nFor more information see %s -h\n", argv[0], argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (!*(argv + optind))
    {
        fprintf(stderr, "Usage: %s [-tfdh] <input>\nFor more information see %s -h\n", argv[0], argv[0]);
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

    size_t s = strlen(input);
    while (true) {
        printf("%s\n", input);
        input = cycle(input, s);
        #ifdef _WIN32
        Sleep(delay/1000);
        #else
        usleep(delay);
        #endif
    }
}