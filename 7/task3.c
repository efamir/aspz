#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <word> <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *search_word = argv[1];
    const char *filename = argv[2];

    FILE *fp; // File pointer
    char line_buffer[MAX_LINE_LENGTH];

    fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        if (strstr(line_buffer, search_word) != NULL) {
            printf("%s", line_buffer);
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}