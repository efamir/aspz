#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // Step 1: Create a file as a regular user
    FILE *file = fopen("/tmp/original.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error creating /tmp/original.txt\n");
        return 1;
    }
    fprintf(file, "Hello\n");
    fclose(file);
    printf("Created /tmp/original.txt as regular user\n");

    // Step 2: Copy file to home directory as root using sudo
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Error getting home directory\n");
        unlink("/tmp/original.txt");
        return 1;
    }
    char copy_cmd[256];
    snprintf(copy_cmd, sizeof(copy_cmd), "sudo cp /tmp/original.txt %s/copied.txt", home);
    int result = system(copy_cmd);
    if (result != 0) {
        fprintf(stderr, "Error copying file to %s/copied.txt\n", home);
        unlink("/tmp/original.txt");
        return 1;
    }
    printf("Copied to %s/copied.txt as root\n", home);

    // Step 3: Attempt to modify the copied file as regular user
    char copied_path[256];
    snprintf(copied_path, sizeof(copied_path), "%s/copied.txt", home);
    file = fopen(copied_path, "a");
    if (file == NULL) {
        fprintf(stderr, "Cannot modify %s/copied.txt: Permission denied\n", home);
    } else {
        fprintf(file, "Modified\n");
        fclose(file);
        printf("Modified %s/copied.txt as regular user\n", home);
    }

    // Step 4: Attempt to delete the copied file as regular user
    char rm_cmd[256];
    snprintf(rm_cmd, sizeof(rm_cmd), "rm %s/copied.txt", home);
    result = system(rm_cmd);
    if (result != 0) {
        fprintf(stderr, "Error deleting %s/copied.txt\n", home);
    } else {
        printf("Deleted %s/copied.txt as regular user\n", home);
    }

    unlink("/tmp/original.txt");
    return 0;
}
