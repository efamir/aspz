#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

// Main function
int main() {
    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    // Set write end of the pipe to non-blocking mode
    if (fcntl(pipefd[1], F_SETFL, O_NONBLOCK) == -1) {
        perror("Error setting non-blocking mode");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    // Prepare a large buffer (128 KB) to exceed pipe capacity
    size_t nbytes = 128 * 1024; // 128 KB
    char *buffer = malloc(nbytes);
    if (!buffer) {
        perror("Error allocating buffer");
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }
    memset(buffer, 'A', nbytes);

    // Attempt to write 128 KB to the pipe
    ssize_t count = write(pipefd[1], buffer, nbytes);
    if (count == -1) {
        perror("Error writing to pipe");
        free(buffer);
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    // Check if the number of bytes written matches the requested amount
    printf("Requested to write: %zu bytes\n", nbytes);
    printf("Actually written: %zd bytes\n", count);

    // Clean up
    free(buffer);
    close(pipefd[0]);
    close(pipefd[1]);
    return 0;
}
