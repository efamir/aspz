#include <stdio.h>
#include <stdlib.h>
#include <time.h> // clock_gettime, struct timespec

void code_snippet_to_measure() {
    // Example: a loop performing some computation
    volatile long long sum = 0;
    for (long long i = 0; i < 100000000; ++i) {
        sum += i;
    }
}

int main() {
    struct timespec time_start, time_end;
    double elapsed_ms;

    if (clock_gettime(CLOCK_MONOTONIC, &time_start) == -1) {
        perror("clock_gettime start");
        return EXIT_FAILURE;
    }

    code_snippet_to_measure();

    if (clock_gettime(CLOCK_MONOTONIC, &time_end) == -1) {
        perror("clock_gettime end");
        return EXIT_FAILURE;
    }

    long seconds = time_end.tv_sec - time_start.tv_sec;
    long nanoseconds = time_end.tv_nsec - time_start.tv_nsec;

    // Adjust nanoseconds if they are negative
    if (nanoseconds < 0) {
        seconds--;
        nanoseconds += 1000000000;
    }

    elapsed_ms = (seconds * 1000.0) + (nanoseconds / 1000000.0);

    printf("Execution time: %.3f ms\n", elapsed_ms);

    return EXIT_SUCCESS;
}
