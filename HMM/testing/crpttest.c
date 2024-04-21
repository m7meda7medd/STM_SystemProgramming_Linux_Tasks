#include <stdio.h>
#include "../hmm.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For usleep() function
#include <string.h>
#include <stdint.h> // For uintptr_t
#include <stdbool.h>

#define NUM_THREADS 10
#define MAX_ALLOCS_PER_THREAD 1000
#define MAX_ALLOC_SIZE 1000
#define MIN_ALLOC_SIZE 1
#define DEALLOC_PROBABILITY 0.5 // Probability of deallocating memory after each allocation

// Function to generate a random number between min and max (inclusive)
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to introduce memory corruption
void introduce_corruption(void* ptr, size_t size) {
    // Generate a random index for corruption
    int index = random_range(0, size - 1);

    // Corrupt a portion of the memory
    memset((char*)ptr + index, 0xFF, sizeof(char));
}

// Thread function to allocate, access, and deallocate memory randomly
void* allocate_access_deallocate_memory(void* arg) {
    for (int i = 0; i < MAX_ALLOCS_PER_THREAD; i++) {
        // Randomly decide whether to allocate memory
        if ((double)rand() / RAND_MAX < DEALLOC_PROBABILITY) {
            // Allocate memory
            size_t size = random_range(MIN_ALLOC_SIZE, MAX_ALLOC_SIZE);
            void* ptr = malloc(size);
            if (ptr == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                pthread_exit(NULL);
            }

            // Introduce memory corruption
            introduce_corruption(ptr, size);

            // Free the allocated memory after a short delay
            usleep(1000); // Sleep for 1000 microseconds (1 millisecond)
            free(ptr);
        }
    }
    pthread_exit(NULL);
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Create threads to allocate, access, and deallocate memory randomly
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, allocate_access_deallocate_memory, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    printf("Random allocation and deallocation test passed!\n");

    return 0;
}

