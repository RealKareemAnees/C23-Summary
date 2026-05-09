#include <stdlib.h>
#include <stdio.h>

// ============================================================
// Comprehensive Example: Program Structure, Declarations,
// Definitions, and Statements
// ============================================================

// This program demonstrates:
// 1. Proper main() function signature with parameter declaration
// 2. Local variable declarations with initialization (definitions)
// 3. Array declaration with designated initializers
// 4. For loop: iteration with loop variable scoped to the loop
// 5. Function calls (printf) with proper arguments
// 6. Arithmetic operations (multiplication, comparison)
// 7. Scope management (file scope, block scope, loop scope)
// 8. Control flow (function return with status code)
// 9. Proper handling of declared vs. defined entities

// Global constant (file scope)
// Demonstrates: top-level declaration and definition
static const int MAX_ITERATIONS = 5;

// Forward function declaration (optional, for demonstration)
// Demonstrates: declaration without definition
void print_section_header(const char* header);

// ============================================================
// Main Program Entry Point
// ============================================================

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    // ========================================
    // Section 1: Local Variable Declarations with Initialization
    // ========================================

    // Declaration + initializer = definition
    // This declares sensor_data and allocates storage for an array of 5 doubles
    double sensor_data[5] = {
        [0] = 23.5,    // Designated initializer: explicitly set index 0
        [1] = 24.1,    // Designated initializer: explicitly set index 1
        [4] = 22.8,    // Designated initializer: explicitly set index 4
        // Indices [2] and [3] are not explicitly listed
        // In Modern C (C23), they default to 0.0
    };

    print_section_header("Raw Sensor Data");

    // ========================================
    // Section 2: For Loop with Domain Iteration
    // ========================================

    // Loop variable i is declared and scoped to this for loop
    // Demonstrates: proper loop variable declaration in for initialization
    // Demonstrates: proper bounds checking (i < 5 ensures valid array indices)
    for (size_t i = 0; i < 5; ++i) {
        // At each iteration, i takes values 0, 1, 2, 3, 4
        // Loop condition (i < 5) ensures i never reaches 5 (out of bounds)

        // sensor_data[i] accesses array element at index i
        // This is safe because i is in range [0, 4]

        printf("Sensor %zu: %.1f degrees C\n", i, sensor_data[i]);

        // ====================================
        // Edge Case Handling: Scope Awareness
        // ====================================

        // If the loop condition were i < 10 (incorrect), attempting
        // sensor_data[i] at i = 5, 6, 7, 8, 9 would access memory
        // outside the array (undefined behavior).
        // We prevent this with correct bounds i < 5.
    }

    // ========================================
    // Scope End: Loop Variable i is Inaccessible
    // ========================================

    // The identifier 'i' is no longer accessible here.
    // Its scope ended at the closing } of the for loop.
    // This is intentional block scoping.

    print_section_header("Statistical Analysis");

    // ========================================
    // Section 3: Multiple Loop Iterations with Independent Scope
    // ========================================

    // Each loop gets its own scope for the loop variable.
    // This is safe and clear.
    for (size_t iter = 0; iter < 5; ++iter) {
        double value = sensor_data[iter];
        double squared = value * value;

        printf("Reading %zu: %.1f, Square: %.1f\n", iter, value, squared);
    }

    // ========================================
    // Section 4: Demonstrating Declaration vs. Definition
    // ========================================

    // Declaration only (no initializer):
    int result;  // This declares result but does NOT initialize it

    // To avoid undefined behavior, we initialize before use:
    result = 42;  // Assignment after declaration; result is now defined with a value

    printf("\nFinal result: %d\n", result);

    // ========================================
    // Section 5: Function Return and Status Code
    // ========================================

    // main() returns int (not void; that would be incorrect in Modern C)
    // EXIT_SUCCESS signals successful program termination to the OS
    // This satisfies the semantic requirement of the main function declaration
    return EXIT_SUCCESS;
}

// ============================================================
// Function Definition: print_section_header
// ============================================================

// Forward declaration at top (demonstrates multiple declarations allowed)
// Actual definition here (demonstrates declaration + body = definition)

void print_section_header(const char* header) {
    // This function was declared forward above (declaration only)
    // Now we define it (provide the implementation in {...})

    printf("\n===== %s =====\n", header);

    // Function ends with implicit return (void return type requires no value)
}

// ============================================================
// Compilation (all these commands are equivalent for this example):
// ============================================================
// gcc -std=c23 -Wall -o program_structure_example program_structure_example.c
// clang -std=c23 -Wall -o program_structure_example program_structure_example.c
// c23 -Wall -o program_structure_example program_structure_example.c
//
// Execution:
// ./program_structure_example
//
// Expected Output:
// ===== Raw Sensor Data =====
// Sensor 0: 23.5 degrees C
// Sensor 1: 24.1 degrees C
// Sensor 2: 0.0 degrees C
// Sensor 3: 0.0 degrees C
// Sensor 4: 22.8 degrees C
//
// ===== Statistical Analysis =====
// Reading 0: 23.5, Square: 552.2
// Reading 1: 24.1, Square: 580.8
// Reading 2: 0.0, Square: 0.0
// Reading 3: 0.0, Square: 0.0
// Reading 4: 22.8, Square: 519.8
//
// Final result: 42
//
// ============================================================
// Key Concepts Embedded in This Code:
// ============================================================
// 1. Proper main() signature and return type
// 2. Function forward declaration (declaration without definition)
// 3. Array declaration with designated initializers
// 4. Scope management: file scope (MAX_ITERATIONS), block scope (sensor_data),
//    loop scope (i, iter)
// 5. Definition = declaration + initializer/implementation
// 6. Loop variables scoped to their for loops (best practice)
// 7. Safe array indexing with loop bounds ensuring valid indices
// 8. Call-by-value semantics in function parameters
// 9. Function definition providing implementation
// 10. Status code return from main() for OS signaling
