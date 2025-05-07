#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("testfile.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Write byte sequence: 4, 5, 2, 2, 3, 3, 7, 9, 1, 5
    unsigned char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    if (write(fd, data, sizeof(data)) != sizeof(data)) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // Move file pointer to position 3
    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("Error seeking in file");
        close(fd);
        return 1;
    }

    // Read 4 bytes into buffer
    unsigned char buffer[4];
    ssize_t count = read(fd, buffer, 4);
    if (count == -1) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }

    printf("Buffer contains: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}
