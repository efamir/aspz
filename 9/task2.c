#include <stdio.h>
#include <stdlib.h>

int main() {
    int result = system("sudo cat /etc/shadow");

    // Check if command failed
    if (result != 0) {
        fprintf(stderr, "Error executing sudo cat /etc/shadow\n");
        return 1;
    }

    return 0;
}
