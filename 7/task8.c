#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h> // stat, remove
#include <errno.h>
#include <ctype.h>  // tolower

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char response_buffer[10];

    printf("Files in current working directory:\n");

    if ((dir = opendir(".")) == NULL) {
        perror("Error opening current directory");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (stat(entry->d_name, &statbuf) == -1) {
            fprintf(stderr, "Error getting status for '%s': ", entry->d_name);
            perror("");
            continue;
        }

        // Process only regular files
        if (S_ISREG(statbuf.st_mode)) {
            printf("File: %s\n", entry->d_name);
            printf("  Delete this file? (y/n): ");
            fflush(stdout);

            if (fgets(response_buffer, sizeof(response_buffer), stdin) != NULL) {
                response_buffer[strcspn(response_buffer, "\n")] = 0; // Remove newline

                if (strlen(response_buffer) == 1 && tolower((unsigned char)response_buffer[0]) == 'y') {
                    if (remove(entry->d_name) == 0) {
                        printf("  File '%s' deleted successfully.\n", entry->d_name);
                    } else {
                        fprintf(stderr, "  Error deleting file '%s': ", entry->d_name);
                        perror("");
                    }
                } else {
                    printf("  File '%s' not deleted.\n", entry->d_name);
                }
            } else {
                // fgets failed
                printf("\n  No valid input received or input error. Skipping file '%s'.\n", entry->d_name);
                 if (feof(stdin)) {
                    clearerr(stdin); // Clear EOF
                 }
            }
        }
    }

    closedir(dir);
    printf("Finished processing files.\n");
    return EXIT_SUCCESS;
}
