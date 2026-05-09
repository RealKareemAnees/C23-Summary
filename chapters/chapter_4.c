#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// ============================================================
// Comprehensive Example: Operators, Expressions, and Evaluation
// ============================================================
// Demonstrates:
// 1. Arithmetic operators (+, -, *, /, %)
// 2. Assignment and compound assignment (=, +=, -=, etc.)
// 3. Increment/decrement (++, --)
// 4. Comparison operators (==, !=, <, >, <=, >=)
// 5. Logical operators (!, &&, ||)
// 6. Ternary conditional operator (? :)
// 7. Short-circuit evaluation
// 8. Side effects and expression purity
// 9. Size_t arithmetic and wrap-around semantics
// 10. Operator evaluation order

int main(void) {
    // ========================================
    // Section 1: Arithmetic Operations on size_t
    // ========================================

    printf("=== Arithmetic Operators ===\n");

    size_t a = 45;
    size_t b = 7;

    size_t sum = a + b;        // 52
    size_t diff = a - b;       // 38
    size_t prod = (a - b) * 2; // 76

    printf("a = %zu, b = %zu\n", a, b);
    printf("a + b = %zu\n", sum);
    printf("a - b = %zu\n", diff);
    printf("(a - b) * 2 = %zu\n", prod);

    // Division and remainder
    size_t dividend = 14;
    size_t divisor = 3;
    size_t quotient = dividend / divisor;   // 4
    size_t remainder = dividend % divisor;  // 2

    printf("\n%zu / %zu = %zu (quotient)\n", dividend, divisor, quotient);
    printf("%zu %% %zu = %zu (remainder)\n", dividend, divisor, remainder);
    printf("Verify: %zu * %zu + %zu = %zu\n", quotient, divisor, remainder,
           quotient * divisor + remainder);

    // ========================================
    // Section 2: Assignment and Compound Assignment
    // ========================================

    printf("\n=== Assignment Operators ===\n");

    size_t counter = 10;
    printf("Initial: counter = %zu\n", counter);

    counter += 5;   // 15
    printf("After += 5: counter = %zu\n", counter);

    counter -= 3;   // 12
    printf("After -= 3: counter = %zu\n", counter);

    counter *= 2;   // 24
    printf("After *= 2: counter = %zu\n", counter);

    counter /= 4;   // 6
    printf("After /= 4: counter = %zu\n", counter);

    // ========================================
    // Section 3: Increment and Decrement
    // ========================================

    printf("\n=== Increment/Decrement Operators ===\n");

    size_t i = 5;
    size_t j;

    // Prefix: increment, then use value
    j = ++i;  // i becomes 6, j = 6
    printf("i = %zu, j = ++i: i=%zu, j=%zu\n", i, i, j);

    i = 5;
    // Postfix: use value, then increment
    j = i++;  // j = 5, i becomes 6
    printf("i = %zu, j = i++: i=%zu, j=%zu\n", i, i, j);

    // Loop with prefix increment
    printf("Loop with ++i: ");
    for (size_t idx = 0; idx < 3; ++idx) {
        printf("%zu ", idx);
    }
    printf("\n");

    // ========================================
    // Section 4: Comparison Operators
    // ========================================

    printf("\n=== Comparison Operators ===\n");

    size_t x = 10;
    size_t y = 15;

    printf("x = %zu, y = %zu\n", x, y);
    printf("x == y: %d\n", x == y);  // 0 (false)
    printf("x != y: %d\n", x != y);  // 1 (true)
    printf("x < y:  %d\n", x < y);   // 1 (true)
    printf("x > y:  %d\n", x > y);   // 0 (false)
    printf("x <= y: %d\n", x <= y);  // 1 (true)
    printf("x >= y: %d\n", x >= y);  // 0 (false)

    // Creative use: comparison results as array indices
    printf("\nComparison as array index:\n");
    int sign_count[2] = {0, 0};
    double values[5] = {-1.5, 0.0, 2.3, -0.5, 1.0};

    for (size_t idx = 0; idx < 5; ++idx) {
        // (values[idx] >= 0.0) returns 1 (true) or 0 (false)
        sign_count[(values[idx] >= 0.0)] += 1;
    }
    printf("Negative: %d, Non-negative: %d\n", sign_count[0], sign_count[1]);

    // ========================================
    // Section 5: Logical Operators and Short-Circuit
    // ========================================

    printf("\n=== Logical Operators (Short-Circuit) ===\n");

    size_t numerator = 10;
    size_t denominator = 0;

    // Short-circuit prevents division by zero
    printf("numerator = %zu, denominator = %zu\n", numerator, denominator);

    if (denominator != 0 && (numerator / denominator) > 2) {
        printf("Division result > 2\n");
    } else {
        printf("Safe check: denominator was 0, division skipped\n");
    }

    // Logical NOT: negate a scalar condition
    if (!denominator) {
        printf("denominator is zero (evaluates to true when negated)\n");
    }

    // Logical OR: returns 1 if either operand is true
    printf("denominator != 0 || numerator > 5: %d\n",
           (denominator != 0) || (numerator > 5));

    // ========================================
    // Section 6: Ternary Conditional Operator
    // ========================================

    printf("\n=== Ternary Conditional Operator ===\n");

    size_t min_val = (x < y) ? x : y;  // Returns x (10) since x < y
    printf("min(%zu, %zu) = %zu\n", x, y, min_val);

    // Nested ternary for multi-way logic
    int grade = 85;
    const char* level = (grade >= 90) ? "A" :
                        (grade >= 80) ? "B" :
                        (grade >= 70) ? "C" : "F";
    printf("Grade %d: %s\n", grade, level);

    // ========================================
    // Section 7: Side Effects and Evaluation Order
    // ========================================

    printf("\n=== Side Effects and Evaluation Order ===\n");

    // Safe: no side effects; evaluation order doesn't matter
    size_t result_pure = (5 + 3) * 2;
    printf("(5 + 3) * 2 = %zu\n", result_pure);

    // Problematic: side effects in an expression
    // AVOID THIS PATTERN:
    // size_t p = 5, q = 3;
    // size_t result = p++ + q++;  // Evaluation order unclear; confusing

    // Better: separate statements
    size_t p = 5;
    size_t q = 3;
    size_t result = p + q;  // 8
    ++p;  // Now p = 6
    ++q;  // Now q = 4
    printf("p = %zu, q = %zu, p + q (original) = %zu\n", p, q, result);

    // ========================================
    // Section 8: size_t Wrap-Around on Underflow
    // ========================================

    printf("\n=== size_t Wrap-Around Semantics ===\n");

    size_t zero = 0;
    size_t one = 1;

    // Underflow wraps to SIZE_MAX
    size_t wrapped = zero - one;  // Wraps to SIZE_MAX
    printf("0 - 1 (as size_t) = %zu\n", wrapped);
    printf("SIZE_MAX = %zu\n", (size_t)-1);  // SIZE_MAX
    printf("Verify: 0 - 1 == SIZE_MAX: %d\n", wrapped == (size_t)-1);

    // ========================================
    // Section 9: Avoiding Common Pitfalls
    // ========================================

    printf("\n=== Avoiding Pitfalls ===\n");

    // Safe modulo operation with explicit zero check
    size_t mod_divisor = 5;
    for (size_t mod_val = 0; mod_val <= 12; ++mod_val) {
        if (mod_divisor != 0) {
            printf("%zu %% %zu = %zu\n", mod_val, mod_divisor, mod_val % mod_divisor);
        }
    }

    // ========================================
    // Success
    // ========================================

    printf("\n=== Program completed successfully ===\n");
    return EXIT_SUCCESS;
}

// ============================================================
// Compilation:
// ============================================================
// gcc -std=c23 -Wall -O2 -o operators_example operators_example.c
// clang -std=c23 -Wall -O2 -o operators_example operators_example.c
//
// Execution:
// ./operators_example
// Key Concepts Demonstrated:
// ============================================================
// 1. Arithmetic on unsigned types (size_t)
// 2. Division, modulo, and their properties
// 3. Assignment and compound assignment operators
// 4. Prefix and postfix increment (different return values)
// 5. Comparison operators returning 0 or 1
// 6. Logical operators (!, &&, ||) and short-circuit evaluation
// 7. Ternary conditional expression
// 8. Short-circuit prevention of undefined behavior
// 9. Side effects and their risks
// 10. Wrap-around semantics on size_t underflow
// 11. Pure expressions vs. side-effect expressions
// 12. Evaluation order unpredictability for most operators
