#include <stdio.h>
#include <stdlib.h>

#define LINES_PER_PAGE 20 // Number of lines to display before pausing

// Function declaration
void display_file(const char *filename);

int main(int argc, char *argv[]) {
    // Check if at least one filename is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2] ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        display_file(argv[i]);
        if (i < argc - 1) {
             printf("\n----------------------------------------\n");
        }
    }

    return EXIT_SUCCESS;
}

void display_file(const char *filename) {
    FILE *fp;
    char line_buffer[1024];
    int line_count = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return;
    }

    printf("--- File: %s ---\n", filename);

    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        printf("%s", line_buffer);
        line_count++;

        if (line_count >= LINES_PER_PAGE) {
            printf("--More-- (Press Enter to continue)");
            fflush(stdout);
            while (getchar() != '\n');

            line_count = 0;
        }
    }

    fclose(fp);
    printf("\n--- End of file: %s ---\n", filename);
}
