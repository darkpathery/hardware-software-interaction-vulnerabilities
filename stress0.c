#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/sysinfo.h>

// Function to generate a simple repeating binary pattern (Simple Shape: Alternating 1s and 0s)
void generate_simple_shape(char *layer, int size) {
    for (int i = 0; i < size; i++) {
        layer[i] = (i % 2 == 0) ? '1' : '0';  // Alternating 1s and 0s
    }
    layer[size] = '\0';
}

// Function to generate a complex pseudo-random binary pattern (Complex Shape: Random 1s and 0s)
void generate_complex_shape(char *layer, int size) {
    for (int i = 0; i < size; i++) {
        layer[i] = (rand() % 2) ? '1' : '0';  // Random 1s and 0s
    }
    layer[size] = '\0';
}

// Function to generate a circular binary pattern (for higher-spec systems)
void generate_circle_shape(char *layer, int size) {
    for (int i = 0; i < size; i++) {
        // Create a circle-like pattern by alternating sections
        if (i < size / 4 || (i >= size / 2 && i < 3 * size / 4)) {
            layer[i] = '1';
        } else {
            layer[i] = '0';
        }
    }
    layer[size] = '\0';
}

// Function to simulate stress by performing an operation on binary data
void stress_system(int layers, int size, int complexity) {
    char *layer = (char *)malloc(size + 1);
    if (layer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < layers; i++) {
        // Generate binary layer based on complexity and shape
        if (complexity == 0) {
            generate_simple_shape(layer, size);  // Simpler shape for low-spec systems
        } else if (complexity == 1) {
            generate_complex_shape(layer, size);  // More complex shape for high-spec systems
        } else {
            generate_circle_shape(layer, size);  // Circle shape for very high-spec systems
        }

        // Simulate an operation: converting binary string to integer and squaring it
        long long int number = 0;
        for (int j = 0; j < size; j++) {
            if (layer[j] == '1') {
                number += (1LL << (size - j - 1));  // Convert binary string to number
            }
        }

        // Perform an operation to keep the CPU busy (square the number)
        long long int result = number * number;

        // Add a small delay to simulate real-time processing
        usleep(100);  // 0.1 milliseconds
    }

    free(layer);
}

// Function to get system specifications
void get_system_specs(int *cpu_cores, long *memory) {
    *cpu_cores = sysconf(_SC_NPROCESSORS_ONLN);
    struct sysinfo sys_info;
    sysinfo(&sys_info);
    *memory = sys_info.totalram / (1024 * 1024 * 1024);  // Memory in GB
}

// Function to adjust stress based on system specs
void adjust_stress_based_on_specs(int cpu_cores, long memory, int *layers, int *size, int *complexity) {
    if (memory <= 4) {  // Low-spec machine (e.g., <= 4GB RAM)
        *layers = 50;
        *size = 1000;
        *complexity = 0;  // Simpler shape
    } else if (memory <= 8) {  // Mid-spec machine (e.g., 4-8GB RAM)
        *layers = 100;
        *size = 2000;
        *complexity = 0;  // Simpler shape
    } else {  // High-spec machine (e.g., > 8GB RAM)
        *layers = 150;
        *size = 3000;
        *complexity = 2;  // Complex shape (circle shape for very high-spec systems)
    }
}

int main() {
    srand(time(NULL));  // Seed random number generator
    
    // Get system specifications
    int cpu_cores;
    long memory;
    get_system_specs(&cpu_cores, &memory);

    printf("Detected %d CPU cores, %ld GB RAM.\n", cpu_cores, memory);

    // Adjust stress based on system specs
    int layers, size, complexity;
    adjust_stress_based_on_specs(cpu_cores, memory, &layers, &size, &complexity);
    
    printf("Adjusting stress test: %d layers, each %d bits of data, complexity %d.\n", layers, size, complexity);

    // Start the stress test
    stress_system(layers, size, complexity);

    return 0;
}
