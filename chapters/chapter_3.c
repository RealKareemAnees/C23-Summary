#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

// ============================================================
// Comprehensive Example: Control Flow, Iteration, and Selection
// ============================================================
// Demonstrates:
// 1. Conditional execution (if/else)
// 2. Domain iteration (for, while, do loops)
// 3. Loop control (break, continue)
// 4. Multi-way selection (switch)
// 5. Scalar truth value evaluation
// 6. Proper loop bounds and variable scoping

// Example function: compute multiplicative inverse using Heron's method
double compute_inverse(double a) {
    // Precision threshold
    constexpr double epsilon = 1E-12;

    // Initial estimate
    double x = 0.5;

    // Iteration counter for safety (prevents infinite loops)
    int iterations = 0;
    constexpr int max_iterations = 1000;

    // ========================================
    // Heron's method: iterative refinement
    // ========================================
    // Loop until convergence or max iterations reached
    while (iterations < max_iterations) {
        double prod = a * x;

        // Check convergence: product close to 1.0?
        if (fabs(1.0 - prod) < epsilon) {
            // Converged; break out of loop
            break;
        }

        // Heron iteration: x_new = x * (2 - a*x)
        x *= (2.0 - prod);

        // Increment counter
        ++iterations;
    }

    return x;
}

// ========================================
// Main Program: Demonstrate Control Structures
// ========================================

int main(int argc, char* argv[argc + 1]) {

    // ========================================
    // Section 1: Conditional Execution (if/else)
    // ========================================

    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // ========================================
    // Section 2: Domain Iteration (for loop)
    // ========================================

    // Process command-line arguments
    for (int arg_idx = 1; arg_idx < argc; ++arg_idx) {
        // Edge case: skip empty arguments
        if (!argv[arg_idx]) {
            continue;  // Skip to next iteration
        }

        // Convert string to double
        char* end_ptr = NULL;
        double value = strtod(argv[arg_idx], &end_ptr);

        // Validate conversion
        if (argv[arg_idx] == end_ptr) {
            printf("Warning: Could not parse '%s' as a number; skipping.\n", argv[arg_idx]);
            continue;  // Skip this argument
        }

        // Skip zero (division by zero is undefined)
        if (value == 0.0) {
            printf("Cannot compute inverse of zero; skipping.\n");
            continue;
        }

        // Compute inverse
        double inverse = compute_inverse(value);
        double product = value * inverse;

        printf("Value: %.5e, Inverse: %.5e, Product: %.15f\n", value, inverse, product);
    }

    // ========================================
    // Section 3: Alternative Loop Forms (while, do)
    // ========================================

    printf("\nDemonstrating loop forms:\n");

    // while loop: pre-test (may not execute at all)
    int counter = 0;
    printf("While loop (counts to 5):\n");
    while (counter < 5) {
        printf("  Counter: %d\n", counter);
        ++counter;
    }

    // do loop: post-test (always executes at least once)
    int value_to_process = 0;
    printf("Do-while loop (always executes once):\n");
    do {
        printf("  Processing value: %d\n", value_to_process);
        ++value_to_process;
    } while (value_to_process < 1);  // Even though condition is false initially

    // ========================================
    // Section 4: Multi-Way Selection (switch)
    // ========================================

    printf("\nDemonstrating switch with fall-through:\n");

    for (int level = 0; level <= 3; ++level) {
        printf("Confidence level %d: ", level);

        switch (level) {
            default:
                printf("Unknown\n");
                break;  // Terminates switch

            case 0:
                printf("No ");
                // Fall through: intentional (no break)
            case 1:
                printf("Low ");
                // Fall through: intentional (no break)
            case 2:
                printf("confidence\n");
                break;  // Terminates switch

            case 3:
                printf("High confidence\n");
                break;  // Terminates switch
        }
    }

    // ========================================
    // Section 5: Scalar Truth Values
    // ========================================

    printf("\nDemonstrating scalar truth values:\n");

    double array[5] = {0.0, 1.5, 0.0, 2.3, 0.0};
    int nonzero_count = 0;

    // Iterate and count non-zero elements using scalar conditions
    for (size_t i = 0; i < 5; ++i) {
        // Direct scalar evaluation: if (array[i]) is true if non-zero
        if (array[i]) {
            printf("  Element %zu is non-zero: %g\n", i, array[i]);
            ++nonzero_count;
        }
    }

    printf("Total non-zero elements: %d\n", nonzero_count);

    // ========================================
    // Section 6: Infinite Loop with Explicit Break
    // ========================================

    printf("\nDemonstrating infinite loop with explicit break:\n");

    size_t iterations = 0;
    for (;;) {  // Equivalent to while (true)
        ++iterations;

        // Explicit termination condition inside loop
        if (iterations >= 5) {
            printf("  Breaking after %zu iterations\n", iterations);
            break;  // Exits the loop
        }

        printf("  Iteration %zu\n", iterations);
    }

    // ========================================
    // Success
    // ========================================

    return EXIT_SUCCESS;
}

// ============================================================
// Compilation:
// ============================================================
// gcc -std=c23 -Wall -O2 -o control_flow_example control_flow_example.c -lm
// clang -std=c23 -Wall -O2 -o control_flow_example control_flow_example.c -lm
//
// Execution:
// ./control_flow_example 0.5 2.0 0.1 3.14159
//
// Key Concepts Demonstrated:
// ============================================================
// 1. if/else: conditional branching based on scalar conditions
// 2. for loop: domain iteration with proper loop variable scoping
// 3. while loop: pre-test loop (may not execute)
// 4. do-while loop: post-test loop (always executes at least once)
// 5. break: explicitly terminates a loop
// 6. continue: skips to loop increment and condition re-evaluation
// 7. switch/case: multi-way branching with fall-through semantics
// 8. Scalar truth values: direct use in conditions without explicit comparison to 0
// 9. Infinite loops: for(;;) pattern with explicit termination via break
// 10. Loop bounds: proper handling of zero-indexed arrays and iteration ranges
// 11. Initialization: ensuring loop variables and counters are properly initialized
// 12. Uninitialized variable prevention through loop construction
