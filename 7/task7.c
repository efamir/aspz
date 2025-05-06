#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h> // For getuid, chmod
#include <errno.h>
#include <ctype.h>  // For tolower

// Forward declarations
void find_and_process_c_sources(const char *basePath, uid_t currentUserUid);
void ask_and_set_read_permission(const char *filePath, mode_t currentMode);

int main() {
    uid_t currentUserUid = getuid();
    printf("Searching for your C source files (.c) in the current directory and subdirectories...\n");
    find_and_process_c_sources(".", currentUserUid);
    return EXIT_SUCCESS;
}

void find_and_process_c_sources(const char *basePath, uid_t currentUserUid) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024]; // Buffer for constructing file paths

    if ((dir = opendir(basePath)) == NULL) {
        fprintf(stderr, "Error opening directory '%s': ", basePath);
        perror(""); // Print system error message
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construct full path
        snprintf(path, sizeof(path), "%s/%s", basePath, entry->d_name);

        // Get file status
        if (stat(path, &statbuf) == -1) {
            fprintf(stderr, "Error getting status for '%s': ", path);
            perror("");
            continue;
        }

        // If it's a directory, recurse
        if (S_ISDIR(statbuf.st_mode)) {
            find_and_process_c_sources(path, currentUserUid);
        } else if (S_ISREG(statbuf.st_mode)) { // If it's a regular file
            // Check if the file is owned by the current user
            if (statbuf.st_uid == currentUserUid) {
                const char *extension = strrchr(entry->d_name, '.');
                // Check if the file has a .c extension
                if (extension != NULL && strcmp(extension, ".c") == 0) {
                    printf("Found your C source file: %s\n", path);
                    ask_and_set_read_permission(path, statbuf.st_mode);
                }
            }
        }
    }

    closedir(dir);
}

void ask_and_set_read_permission(const char *filePath, mode_t currentMode) {
    char response_buffer[10];
    int change_permission = 0;

    // Check if read permission for others is already set
    if (currentMode & S_IROTH) {
        printf("  Read permission for others is already set for '%s'.\n", filePath);
        return; 
    }

    printf("  Do you want to grant read permission to others for '%s'? (y/n): ", filePath);
    fflush(stdout); // Ensure prompt is displayed before reading input

    if (fgets(response_buffer, sizeof(response_buffer), stdin) != NULL) {
        // Remove newline character if present
        response_buffer[strcspn(response_buffer, "\n")] = 0; 
        // Check if the input is 'y' (case-insensitive) and only one character
        if (strlen(response_buffer) == 1 && tolower((unsigned char)response_buffer[0]) == 'y') {
            change_permission = 1;
        }
    } else {
        // fgets failed (e.g., EOF or error)
        printf("\n  No valid input received or input error. Skipping permission change for '%s'.\n", filePath);
        if (feof(stdin)) { // If EOF, clear it for potential future inputs in other contexts
            clearerr(stdin);
        }
        return;
    }

    if (change_permission) {
        mode_t new_mode = currentMode | S_IROTH; // Add S_IROTH permission
        if (chmod(filePath, new_mode) == 0) {
            printf("  Successfully granted read permission to others for '%s'.\n", filePath);
        } else {
            fprintf(stderr, "  Error setting read permission for '%s': ", filePath);
            perror("");
        }
    } else {
        printf("  Read permission for others not changed for '%s'.\n", filePath);
    }
}
