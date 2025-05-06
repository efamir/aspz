#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h> // stat()
#include <string.h>
#include <errno.h>

int compare_strings(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char **subdir_names = NULL;
    int count = 0;
    int capacity = 10;

    subdir_names = malloc(capacity * sizeof(char *));

    if ((dir = opendir(".")) == NULL) {
        perror("Error opening current directory");
        free(subdir_names);
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (stat(entry->d_name, &statbuf) == -1) {
             fprintf(stderr, "Warning: Cannot stat '%s': ", entry->d_name);
             perror("");
            continue;
        }

        // Check if it's a directory
        if (S_ISDIR(statbuf.st_mode)) {
            // Check if we need more space in our dynamic array
            if (count >= capacity) {
                capacity *= 2; // Double the capacity
                char **temp = realloc(subdir_names, capacity * sizeof(char *));
                if (temp == NULL) {
                    perror("Failed to reallocate memory for subdir_names");
                    for (int i = 0; i < count; i++) {
                        free(subdir_names[i]);
                    }
                    free(subdir_names);
                    closedir(dir);
                    return EXIT_FAILURE;
                }
                subdir_names = temp;
            }

            // Duplicate the directory
            subdir_names[count] = strdup(entry->d_name);
            if (subdir_names[count] == NULL) {
                 perror("Failed to duplicate directory name");
                 for (int i = 0; i < count; i++) {
                     free(subdir_names[i]);
                 }
                 free(subdir_names);
                 closedir(dir);
                 return EXIT_FAILURE;
            }
            count++;
        }
    }
    closedir(dir);

    if (count > 0) {
        qsort(subdir_names, count, sizeof(char *), compare_strings);
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", subdir_names[i]);
        free(subdir_names[i]);
    }

    free(subdir_names);

    return EXIT_SUCCESS;
}