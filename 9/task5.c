#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void check_access(const char *path) {
    // Check read access
    FILE *file = fopen(path, "r");
    if (file) {
        printf("File %s: Read access granted\n", path);
        fclose(file);
    } else {
        printf("File %s: Read access denied\n", path);
    }

    // Check write access
    file = fopen(path, "a");
    if (file) {
        printf("File %s: Write access granted\n", path);
        fclose(file);
    } else {
        printf("File %s: Write access denied\n", path);
    }
}

int main() {
    const char *file_path = "/tmp/testfile.txt";

    // Step 1: Create a temporary file as regular user
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        fprintf(stderr, "Error creating %s\n", file_path);
        return 1;
    }
    fprintf(file, "Initial content\n");
    fclose(file);
    printf("Created %s as regular user\n", file_path);

    printf("\nChecking initial access:\n");
    check_access(file_path);

    // Step 2: Change ownership to nobody and set permissions to 600 as root
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "sudo chown nobody %s", file_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error changing ownership of %s\n", file_path);
        unlink(file_path);
        return 1;
    }
    snprintf(cmd, sizeof(cmd), "sudo chmod 600 %s", file_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error setting permissions on %s\n", file_path);
        unlink(file_path);
        return 1;
    }
    printf("\nChanged ownership to nobody, permissions to 600\n");
    check_access(file_path);

    // Step 3: Change permissions to 400 as root
    snprintf(cmd, sizeof(cmd), "sudo chmod 400 %s", file_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error setting permissions on %s\n", file_path);
        unlink(file_path);
        return 1;
    }
    printf("\nChanged permissions to 400\n");
    check_access(file_path);

    // Step 4: Change permissions to 000 as root
    snprintf(cmd, sizeof(cmd), "sudo chmod 000 %s", file_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error setting permissions on %s\n", file_path);
        unlink(file_path);
        return 1;
    }
    printf("\nChanged permissions to 000\n");
    check_access(file_path);

    snprintf(cmd, sizeof(cmd), "sudo rm %s", file_path);
    system(cmd);
    printf("\nCleaned up %s\n", file_path);
    return 0;
}