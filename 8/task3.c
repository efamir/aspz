#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Global counter for comparisons
long long comparisons = 0;

// Comparison function for qsort
int cmp_int(const void *a, const void *b) {
    comparisons++;
    return (*(int *)a - *(int *)b);
}

// Function to measure qsort time and comparisons for an array
double measure_qsort_time(int *arr, int n, int *temp, long long *comp) {
    memcpy(temp, arr, n * sizeof(int)); // Copy to avoid modifying original
    comparisons = 0; // Reset comparison counter
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    qsort(temp, n, sizeof(int), cmp_int);
    clock_gettime(CLOCK_MONOTONIC, &end);
    *comp = comparisons; // Store comparisons
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// Function to check if array is sorted
int is_sorted(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) return 0;
    }
    return 1;
}

// Test function for qsort correctness
void run_tests() {
    printf("Running qsort correctness tests...\n");
    int test1[] = {5, 2, 8, 1, 9};
    qsort(test1, 5, sizeof(int), cmp_int);
    printf("Test 1 (Random array): %s\n", is_sorted(test1, 5) ? "Passed" : "Failed");

    int test2[] = {};
    qsort(test2, 0, sizeof(int), cmp_int);
    printf("Test 2 (Empty array): %s\n", is_sorted(test2, 0) ? "Passed" : "Failed");

    int test3[] = {4, 4, 4, 4};
    qsort(test3, 4, sizeof(int), cmp_int);
    printf("Test 3 (Identical elements): %s\n", is_sorted(test3, 4) ? "Passed" : "Failed");

    int test4[] = {10, 9, 8, 7, 6};
    qsort(test4, 5, sizeof(int), cmp_int);
    printf("Test 4 (Reverse sorted): %s\n", is_sorted(test4, 5) ? "Passed" : "Failed");
}

// Main function
int main() {
    srand(time(NULL));
    int n = 1000000; // Larger array size
    int runs = 5; // Fewer runs for larger array
    int *arr = malloc(n * sizeof(int));
    int *temp = malloc(n * sizeof(int));
    if (!arr || !temp) {
        perror("Memory allocation failed");
        free(arr);
        free(temp);
        return 1;
    }

    printf("Performance tests for qsort (array size = %d, %d runs):\n", n, runs);

    // Test 1: Sorted array
    for (int i = 0; i < n; i++) arr[i] = i;
    double time_sorted = 0;
    long long comp_sorted = 0;
    for (int i = 0; i < runs; i++) {
        long long comp;
        time_sorted += measure_qsort_time(arr, n, temp, &comp);
        comp_sorted += comp;
    }
    printf("Sorted array: %.6f seconds, %lld comparisons (avg)\n", time_sorted / runs, comp_sorted / runs);

    // Test 2: Reverse sorted array
    for (int i = 0; i < n; i++) arr[i] = n - i;
    double time_reverse = 0;
    long long comp_reverse = 0;
    for (int i = 0; i < runs; i++) {
        long long comp;
        time_reverse += measure_qsort_time(arr, n, temp, &comp);
        comp_reverse += comp;
    }
    printf("Reverse sorted array: %.6f seconds, %lld comparisons (avg)\n", time_reverse / runs, comp_reverse / runs);

    // Test 3: All identical elements
    for (int i = 0; i < n; i++) arr[i] = 42;
    double time_identical = 0;
    long long comp_identical = 0;
    for (int i = 0; i < runs; i++) {
        long long comp;
        time_identical += measure_qsort_time(arr, n, temp, &comp);
        comp_identical += comp;
    }
    printf("Identical elements: %.6f seconds, %lld comparisons (avg)\n", time_identical / runs, comp_identical / runs);

    // Test 4: Random array
    for (int i = 0; i < n; i++) arr[i] = rand() % n;
    double time_random = 0;
    long long comp_random = 0;
    for (int i = 0; i < runs; i++) {
        long long comp;
        time_random += measure_qsort_time(arr, n, temp, &comp);
        comp_random += comp;
    }
    printf("Random array: %.6f seconds, %lld comparisons (avg)\n", time_random / runs, comp_random / runs);

    // Test 5: Pivot-tricking array
    for (int i = 0; i < n; i++) arr[i] = (i % 2 == 0) ? i : n - i;
    double time_trick = 0;
    long long comp_trick = 0;
    for (int i = 0; i < runs; i++) {
        long long comp;
        time_trick += measure_qsort_time(arr, n, temp, &comp);
        comp_trick += comp;
    }
    printf("Pivot-tricking array: %.6f seconds, %lld comparisons (avg)\n", time_trick / runs, comp_trick / runs);

    // Run correctness tests
    run_tests();

    // Clean up
    free(arr);
    free(temp);
    return 0;
}