#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

enum Mode {
    FILE_MODE,
    TEXT_MODE
};

void cycle(char* text, int length) {
    char first = text[0];
    for (int i = 1; i < length; i++) {
        text[i - 1] = text[i];
    }
    text[length - 1] = first;
}

int main(int argc, char *argv[]) {
    enum Mode mode = FILE_MODE;
    long delay = 100;

    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            mode = TEXT_MODE;
        } else if (strcmp(argv[i], "-f") == 0) {
            mode = FILE_MODE;
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            delay = atol(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Catroll Util.\n\nUsage: catroll [OPTIONS]\n\nOptions:\n\t-t\t\tText input mode\n\t-f\t\tFile input mode\n\t-d DELAY\tDelay in ms\n\t-h\t\tShow this help\n\n");
            return 0;
        } else {
            fprintf(stderr, "Unknown option '%s'\n", argv[i]);
            return 1;
        }
    }
    char *text = NULL;
    if (mode == FILE_MODE) {
        char *fileName = argv[argc - 1];
        FILE *file;
        errno_t err = fopen_s(&file, fileName, "r");
        if (err != 0) {
            char errBuffer[80];
            strerror_s(errBuffer, 80, err);
            fprintf(stderr, "Can't open file '%s': %s\n", fileName, errBuffer);
            return 1;
        }
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        text = malloc(size + 1);
        long i = 0;
        while (fgets(text + i, size - i, file) != NULL) {
            i = strlen(text);
        }
        fclose(file);
    } else {
        text = argv[argc - 1];
    }
    long length = strlen(text);
    while (1) {
        printf("%s", text);
        cycle(text, length);
        #ifdef _WIN32
        Sleep(delay);
        #else
        usleep(delay * 1000);
        #endif
        printf("\r");
    }
}
