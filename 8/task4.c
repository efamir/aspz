#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    printf("%d\n", pid);

    return 0;
}
