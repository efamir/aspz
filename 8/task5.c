#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


void run_lseek_test(const char *device, int whence, off_t offset, const char *test_name) {
    // Open device
    int fd = open(device, O_RDWR);
    if (fd == -1) {
        printf("Test %s (%s): Failed to open device (%s)\n", test_name, device, strerror(errno));
        return;
    }

    // Perform lseek
    off_t result = lseek(fd, offset, whence);
    if (result == -1) {
        printf("Test %s (%s): lseek failed (%s)\n", test_name, device, strerror(errno));
    } else {
        printf("Test %s (%s): lseek returned %lld\n", test_name, device, (long long)result);
        
        // Try reading 1 byte
        char buffer[1];
        ssize_t read_bytes = read(fd, buffer, 1);
        if (read_bytes == -1) {
            printf("Test %s (%s): Read failed (%s)\n", test_name, device, strerror(errno));
        } else {
            printf("Test %s (%s): Read %zd bytes (value: %d)\n", test_name, device, read_bytes, read_bytes > 0 ? buffer[0] : -1);
        }
    }

    // Close device
    close(fd);
}

int main() {
    printf("Testing lseek with /dev/null and /dev/zero\n");

    // Test 1: SEEK_SET to positive offset
    run_lseek_test("/dev/null", SEEK_SET, 1000, "SEEK_SET positive");
    run_lseek_test("/dev/zero", SEEK_SET, 1000, "SEEK_SET positive");

    // Test 2: SEEK_SET to negative offset
    run_lseek_test("/dev/null", SEEK_SET, -1000, "SEEK_SET negative");
    run_lseek_test("/dev/zero", SEEK_SET, -1000, "SEEK_SET negative");

    // Test 3: SEEK_CUR with positive offset
    run_lseek_test("/dev/null", SEEK_CUR, 500, "SEEK_CUR positive");
    run_lseek_test("/dev/zero", SEEK_CUR, 500, "SEEK_CUR positive");

    // Test 4: SEEK_END with positive offset
    run_lseek_test("/dev/null", SEEK_END, 100, "SEEK_END positive");
    run_lseek_test("/dev/zero", SEEK_END, 100, "SEEK_END positive");

    // Test 5: Write after lseek
    int fd_null = open("/dev/null", O_WRONLY);
    int fd_zero = open("/dev/zero", O_WRONLY);
    if (fd_null == -1 || fd_zero == -1) {
        printf("Write test: Failed to open devices (%s)\n", strerror(errno));
        return 1;
    }

    // Seek and write to /dev/null
    if (lseek(fd_null, 1000, SEEK_SET) != -1) {
        char buf[] = "test";
        ssize_t written = write(fd_null, buf, strlen(buf));
        printf("Write test (/dev/null): Wrote %zd bytes after lseek\n", written);
    } else {
        printf("Write test (/dev/null): lseek failed (%s)\n", strerror(errno));
    }

    // Seek and write to /dev/zero
    if (lseek(fd_zero, 1000, SEEK_SET) != -1) {
        char buf[] = "test";
        ssize_t written = write(fd_zero, buf, strlen(buf));
        printf("Write test (/dev/zero): Wrote %zd bytes after lseek\n", written);
    } else {
        printf("Write test (/dev/zero): lseek failed (%s)\n", strerror(errno));
    }

    // Clean up
    close(fd_null);
    close(fd_zero);
    return 0;
}
