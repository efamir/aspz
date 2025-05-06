#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

void list_directory(const char *path);

int main() {
    list_directory(".");
    return EXIT_SUCCESS;
}

void list_directory(const char *basePath) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    if ((dir = opendir(basePath)) == NULL) {
        fprintf(stderr, "Error opening directory '%s': ", basePath);
        perror("");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore "."  and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct the full path
        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);
        printf("%s\n", path);

        if (stat(path, &statbuf) == -1) {
            fprintf(stderr, "Error getting status for '%s': ", path);
            perror("");
            continue;
        }

        // Check if directory using S_ISDIR macro
        if (S_ISDIR(statbuf.st_mode)) {
            list_directory(path);
        }
    }

    closedir(dir);
}
