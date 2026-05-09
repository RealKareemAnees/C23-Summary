#include <stdlib.h>
#include <stdio.h>

// A comprehensive example demonstrating all key concepts from Chapter 1:
// - main() signature with argc/argv
// - Array declaration and initialization with designated initializers
// - For loop with size_t counter
// - printf() with format strings and format specifiers
// - Proper includes and compiler-friendly patterns
// - Edge case handling and portability

int main(int argc, [[maybe_unused]] char* argv[argc + 1]) {
    // ============================================================
    // Section 1: Array Declaration and Initialization
    // ============================================================

    // Declare a 5-element array of doubles.
    // Designated initializers allow explicit assignment to specific indices.
    // Uninitialized slots default to 0.0 in Modern C.
    // This handles the UB pitfall of uninitialized elements.
    double measurements[5] = {
        [0] = 9.0,        // Index 0: explicitly set
        [1] = 2.9,        // Index 1: explicitly set
        [2] = 0.0,        // Index 2: explicitly set (could omit; defaults to 0.0)
        [3] = 0.00007,    // Index 3: explicitly set (small value in standard notation)
        [4] = 3.E+25,     // Index 4: scientific notation (3 × 10^25)
    };

    // ============================================================
    // Section 2: Loop Iteration with Proper Index Type
    // ============================================================

    // Use size_t for loop counter and array indexing.
    // size_t is the unsigned integer type designed for sizes and indices.
    // This avoids signed/unsigned comparison issues and is portable.
    // Loop runs from i=0 to i=4 (5 iterations for 5-element array).
    // Condition i < 5 is tested before each iteration.
    // ++i (pre-increment) increments before evaluation (minor efficiency edge over i++).
    for (size_t i = 0; i < 5; ++i) {

        // ============================================================
        // Section 3: printf() with Format Specifiers
        // ============================================================

        // printf() is a variadic function accepting format string + arguments.
        // First argument is the format string (a string literal in double quotes).
        // Subsequent arguments are values to insert.

        // Format specifiers:
        //   %zu : format a size_t argument as decimal unsigned integer
        //   %g  : format a double argument in general notation (decimal or exponential)
        //   \t  : escape sequence for horizontal tab
        //   \n  : escape sequence for newline

        // The format string defines expected argument types and count.
        // Three specifiers = three subsequent arguments expected.

        printf(
            "element %zu is %g, \tits square is %g\n",
            i,                // 1st argument: loop counter (size_t i)
            measurements[i],  // 2nd argument: array element at index i (double)
            measurements[i] * measurements[i]  // 3rd argument: computed square (double)
        );

        // ============================================================
        // Edge Case Handling: Index Bounds
        // ============================================================

        // The loop condition i < 5 ensures i never reaches 5.
        // Array indices 0–4 are valid; accessing measurements[5] would be undefined behavior.
        // By using i < array_size, we guarantee safe access.

        // If i erroneously exceeded bounds (e.g., i < 10), this printf would access
        // memory beyond the array boundaries, causing undefined behavior:
        // - Reading garbage values
        // - Corrupting adjacent memory on write
        // - Potential crash
        // We prevent this with the correct loop condition.
    }

    // ============================================================
    // Section 4: Return Status from main()
    // ============================================================

    // main() must return int (not void, which is incorrect in Modern C).
    // EXIT_SUCCESS is a portable constant from <stdlib.h> (typically 0).
    // Returning 0 signals successful execution to the operating system.
    // Non-zero return values indicate failure/error conditions.

    return EXIT_SUCCESS;

    // ============================================================
    // Summary of Concepts Demonstrated:
    // ============================================================
    // 1. Proper main() signature with argc/argv and [[maybe_unused]] attribute
    // 2. Array declaration with designated initializers (avoiding uninitialized UB)
    // 3. For loop with size_t counter (appropriate for indexing)
    // 4. Formatted output with printf() and multiple format specifiers
    // 5. Escape sequences (\t, \n) for output formatting
    // 6. Array indexing with proper bounds checking (loop condition ensures safety)
    // 7. Multiplication operator for computed value in output
    // 8. Correct return status code
    // 9. All required headers included explicitly
    // 10. Compiler-friendly attributes ([[maybe_unused]]) for cleaner builds
}

// ============================================================
// Compilation:
// ============================================================
// gcc -std=c23 -Wall -o chapter1_example chapter1_example.c -lm
// clang -std=c23 -Wall -o chapter1_example chapter1_example.c -lm
// c23 -Wall -o chapter1_example chapter1_example.c -lm
//
// Execution:
// ./chapter1_example
//
// Expected Output (approximate):
// element 0 is 9, 	its square is 81
// element 1 is 2.9, 	its square is 8.41
// element 2 is 0, 	its square is 0
// element 3 is 7e-05, 	its square is 4.9e-09
// element 4 is 3e+25, 	its square is 9e+50
