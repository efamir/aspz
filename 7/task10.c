#include <stdio.h>
#include <stdlib.h> // rand(), srand(), RAND_MAX
#include <time.h>

#define NUM_SAMPLES 5

int main() {
    srand(time(NULL));

    printf("Random numbers between 0.0 and 1.0:\n");
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        double random_val_0_to_1 = (double)rand() / RAND_MAX;
        printf("%.6f\n", random_val_0_to_1);
    }
    printf("\n");

    double n;
    printf("Enter number 'n' (the range will be 0.0 to n): ");
    if (scanf("%lf", &n) != 1) {
        fprintf(stderr, "Invalid input for n.\n");
        return EXIT_FAILURE;
    }

    printf("\nGenerating random numbers between 0.0 and %.2f:\n", n);
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        double random_val_0_to_n = ((double)rand() / RAND_MAX) * n;
        printf("%.6f\n", random_val_0_to_n);
    }

    return EXIT_SUCCESS;
}
