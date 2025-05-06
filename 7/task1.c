#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Not strictly needed for this exact code, but good practice

int main() {
     const char *command = "rwho | more";

    FILE *fp;
    int status; // To get the exit status

    printf("Executing command: %s\n", command);
    fp = popen(command, "r");

    // Check if popen() was successful
    if (fp == NULL) {
        perror("Failed to run popen()");
        return EXIT_FAILURE;
    }

    status = pclose(fp);

    // Check the exit status of the command (optional, but useful)
    if (status == -1) {
        perror("Error in pclose()");
        return EXIT_FAILURE;
    } else {
        printf("Exited successfully\n");
    }

    return EXIT_SUCCESS;
}
