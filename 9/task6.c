#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> // chmod and permission constants
#include <errno.h>    // errno and strerror

// Function to display directory listing and attempt operations on a file
void explore_permissions(const char *dir_path_for_ls, const char *target_file_full_path, const char *file_description) {
    printf("\n--- Listing directory: %s ---\n", dir_path_for_ls);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "ls -l %s", dir_path_for_ls);
    system(cmd);
    printf("\n-- Attempting operations on file: %s (%s) --\n", target_file_full_path, file_description);

    // Attempt to read
    FILE *file_ptr = fopen(target_file_full_path, "r");
    if (file_ptr) {
        printf("Read: SUCCESS\n");
        fclose(file_ptr);
    } else {
        printf("Read: FAILED - %s\n", strerror(errno));
    }

    // Attempt to write (append)
    file_ptr = fopen(target_file_full_path, "a");
    if (file_ptr) {
        printf("Write (append): SUCCESS\n");
        fclose(file_ptr);
    } else {
        printf("Write (append): FAILED - %s\n", strerror(errno));
    }

    // Attempt to check execute permission
    if (access(target_file_full_path, X_OK) == 0) {
        printf("Execute check (X_OK): ALLOWED\n");
    } else {
        printf("Execute check (X_OK): DENIED - %s\n", strerror(errno));
    }

    // Attempt to change permissions
    printf("Attempt chmod 0777: ");
    if (chmod(target_file_full_path, S_IRWXU | S_IRWXG | S_IRWXO) == 0) { // 0777
        printf("SUCCESS\n");
    } else {
        printf("FAILED - %s\n", strerror(errno));
    }
}

int main() {
    const char *home_dir = getenv("HOME");
    if (!home_dir) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        return 1;
    }

    char my_temp_file_path[512];
    snprintf(my_temp_file_path, sizeof(my_temp_file_path), "%s/my_permission_test_file.txt", home_dir);

    FILE* temp_f_ptr = fopen(my_temp_file_path, "w");
    if (temp_f_ptr) {
        fprintf(temp_f_ptr, "Test content.\n");
        fclose(temp_f_ptr);
        chmod(my_temp_file_path, S_IRWXU); // Set rwx permissions for owner
    } else {
        perror("Failed to create temporary file");
    }

    explore_permissions("~", my_temp_file_path, "file created by the program in the home directory");
    explore_permissions("/usr/bin", "/usr/bin/cat", "system executable /usr/bin/cat");
    explore_permissions("/etc", "/etc/hosts", "system configuration file /etc/hosts");

    remove(my_temp_file_path); // Clean up the temp file

    return 0;
}
