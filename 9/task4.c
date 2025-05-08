#include <stdio.h>
#include <stdlib.h>

int main() {
    // Execute whoami command
    printf("Executing whoami:\n");
    int result = system("whoami");
    if (result != 0) {
        fprintf(stderr, "Error executing whoami\n");
        return 1;
    }

    // Execute id command
    printf("\nExecuting id:\n");
    result = system("id");
    if (result != 0) {
        fprintf(stderr, "Error executing id\n");
        return 1;
    }

    return 0;
}
