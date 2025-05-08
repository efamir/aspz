#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char line[256];
    char *current_user = getenv("USER");
    int found = 0;

    // Open getent passwd command
    fp = popen("getent passwd", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error executing getent passwd\n");
        return 1;
    }

    // Read output line by line
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *username = strtok(line, ":");
        strtok(NULL, ":"); // Skip password field
        char *uid_str = strtok(NULL, ":");
        
        // Check if fields are valid
        if (username == NULL || uid_str == NULL) {
            continue;
        }

        // Convert UID to integer
        int uid = atoi(uid_str);
        
        // Check for regular users (UID >= 1000) and exclude current user
        if (uid >= 1000 && strcmp(username, current_user) != 0) {
            printf("Regular user: %s (UID: %d)\n", username, uid);
            found = 1;
        }
    }

    // Close pipe
    if (pclose(fp) == -1) {
        fprintf(stderr, "Error closing getent passwd\n");
        return 1;
    }

    if (!found) {
        printf("No other regular users found.\n");
    }

    return 0;
}
