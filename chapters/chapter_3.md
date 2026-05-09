# Chapter 3: Everything is About Control

Source: 03-everythingisaboutcontrol.md

> Control flow directives determine program execution paths. C provides five primary control mechanisms: `if` for conditional branching, `for`/`while`/`do` for iterations, and `switch` for multi-way selection. Understanding scalar truth value evaluation, loop control with break/continue, and the semantic differences between iteration forms is essential for writing effective imperative programs.

## 1. Exhaustive Concept Breakdown

### 1.1 Conditional Execution with if/else

The `if` statement enables conditional branching based on a controlling expression. A controlling expression is evaluated in boolean context: the associated code block executes once if true, or zero times if false.

**Basic form**: `if (condition) { block }` executes the block only when condition is true.

**Extended form**: `if (condition) { block_true } else { block_false }` executes one of two mutually exclusive code paths.

**Controlling expressions**: Any scalar value can serve as a condition. Comparison operators (`==`, `!=`, `<`, `>`, `<=`, `>=`) return 0 (false) or 1 (true). Boolean types and integer values are directly usable.

### 1.2 Truth Values and Scalar Context Evaluation

In C, truth value semantics apply to all scalar types (integers, floating-point, pointers, bool):

- **0 represents false**: Any scalar value of 0 evaluates to false in a condition
- **Non-zero represents true**: Any non-zero scalar value evaluates to true

This allows code like `if (i) { ... }` to execute when `i` is non-zero, eliminating redundant comparisons like `if (i != 0)`.

**TAKEAWAY 3.1**: Zero = logical false  
**TAKEAWAY 3.2**: Non-zero = logical true  
**TAKEAWAY 3.3**: Avoid explicit comparisons to 0, false, or true; use the value directly  
**TAKEAWAY 3.4**: All scalar types have truth values

### 1.3 The bool Type and Logical Values

The `bool` type (standard in Modern C, formerly required `<stdbool.h>`) holds truth values:

- `false` = 0 (false value)
- `true` = 1 (true value)

Using `bool` for logical values emphasizes semantic intent over raw integers. However, before C23, the header `<stdbool.h>` was required; this is now obsolete for Modern C.

### 1.4 Domain Iteration with for Loops

The `for` statement iterates over a domain of values, executing a block repeatedly with different values of a loop variable.

**Syntax**: `for (clause1; condition2; expression3) { block }`

**Parts**:

- `clause1`: Initialization; typically declares and initializes the loop variable (e.g., `size_t i = 0`)
- `condition2`: Loop condition; iteration continues while this evaluates true; checked before each iteration
- `expression3`: Post-iteration update; executed after each iteration (e.g., `++i`)
- Block: Code executing repeatedly

**Best practices**:

- Declare loop variables in `clause1` to scope them to that loop
- Use `size_t` for indices over arrays and counts
- Always use compound statements `{...}` for loop bodies

**Counting iterations**: The loop variable takes successive values in the iteration domain. Example: `for (size_t i = 0; i < 5; ++i)` iterates with `i = 0, 1, 2, 3, 4`.

### 1.5 Alternative Iteration: while and do Loops

The `while` loop executes repeatedly while a condition is true: `while (condition) { block }`. The condition is checked _before_ each iteration; if false initially, the block never executes.

The `do` loop executes the block first, then checks the condition: `do { block } while (condition);`. The block executes _at least once_, regardless of the condition.

**Key difference**: `while` may never execute; `do` always executes at least once.

**Syntax note**: `do` requires a semicolon after `while(condition)` to terminate the statement.

### 1.6 Loop Control: break and continue

The `break` statement terminates the enclosing loop immediately, bypassing the remainder of the block and any condition re-evaluation.

The `continue` statement skips remaining statements in the current iteration, re-evaluates the loop condition, and—if true—continues from the start of the block.

**Infinite loops with for**: `for (;;)` is equivalent to `while (true)` (the middle condition is omitted and treated as always true). This pattern is common in C for loops where the termination condition is checked explicitly inside the block via a `break` statement.

### 1.7 Multiple Selection with switch

The `switch` statement provides multi-way branching based on an integer expression value, simplifying cascading `if-else` chains.

**Syntax**: `switch (expression) { case value1: ...; case value2: ...; default: ...; }`

**Semantics**:

- The `expression` is evaluated to an integer value
- Control jumps to the matching `case` label
- Execution continues from that point _until a `break` is encountered or the end of the switch block_
- `default` label handles values not explicitly matched

**Fall-through**: Without a `break` statement, execution continues into the next case. This can be intentional (for shared code across multiple cases) or unintentional (a common source of bugs).

**Restrictions**:

- **TAKEAWAY 3.5**: Case values must be compile-time constant integer expressions (not variables)
- **TAKEAWAY 3.6**: Case values must be unique within a switch statement
- **TAKEAWAY 3.7**: Case labels must not jump over variable definitions

### 1.8 Variable Definitions and switch Scope

Declaring variables directly within a switch block (outside any case label) can lead to uninitialized-variable errors. If execution jumps over a variable definition to a case label, that variable's initializer may not execute, leaving the variable uninitialized.

**Solution**: Wrap case blocks in their own `{...}` scopes to give each variable its own block scope.

### 1.9 Scalar Types and Truth Evaluation Table

Scalar types frequently used in conditions:

| Type              | Category          | Example                       | Truth Value                         |
| ----------------- | ----------------- | ----------------------------- | ----------------------------------- |
| `size_t`          | Unsigned integer  | 0, 5, SIZE_MAX                | 0 = false; non-zero = true          |
| `double`          | Floating-point    | 0.0, 3.14                     | 0.0 = false; non-zero = true        |
| `int` / `signed`  | Signed integer    | -1, 0, 1                      | 0 = false; non-zero = true          |
| `unsigned`        | Unsigned integer  | 0, 255                        | 0 = false; non-zero = true          |
| `bool`            | Boolean           | true, false                   | Explicit true/false values          |
| `char`            | Character         | 'a', '\0'                     | 0 (null char) = false; other = true |
| `ptrdiff_t`       | Signed difference | Negative/positive differences | 0 = false; non-zero = true          |
| `char*` / Pointer | Pointer           | NULL, valid address           | NULL = false; valid address = true  |
| `void*`           | Generic pointer   | NULL, address                 | NULL = false; address = true        |

### 1.10 Imperative vs. Functional Paradigms

C's control structures (conditional, loop, switch, function call) embody imperative programming: explicit sequential instructions for the computer. This contrasts with functional paradigms where computation is specified through expression evaluation rather than state modification.

### 1.11 Control Flow Visualization and Program Tracing

Understanding program flow requires tracking which code paths execute for given inputs. Complex nested conditions, multiple loop types, and switch fall-throughs can make tracing non-obvious. Debugging tools (debuggers, print statements) help visualize actual control flow.

## 2. Syntax, Types & Functions Dictionary

| Entity                         | Type/Category                        | Signature/Form                 | Parameters/Examples               | Return / Behavior                                        |
| ------------------------------ | ------------------------------------ | ------------------------------ | --------------------------------- | -------------------------------------------------------- |
| `if (expr) stmt`               | Control statement                    | —                              | Expression (scalar), statement    | Executes stmt if expr is true (non-zero)                 |
| `if (expr) stmt else stmt2`    | Control statement                    | —                              | Expression, two statements        | Executes stmt if true, stmt2 if false                    |
| `for (init; cond; incr) stmt`  | Loop statement                       | —                              | Initializer, condition, increment | Domain iteration; repeats stmt while cond is true        |
| `while (cond) stmt`            | Loop statement                       | —                              | Condition                         | Pre-test loop; executes stmt while cond is true          |
| `do stmt while (cond);`        | Loop statement                       | —                              | Condition                         | Post-test loop; executes stmt at least once              |
| `break;`                       | Loop control                         | —                              | —                                 | Exits enclosing loop immediately                         |
| `continue;`                    | Loop control                         | —                              | —                                 | Skips to loop increment/condition re-evaluation          |
| `switch (expr) { ... }`        | Multi-way selection                  | —                              | Integer expression                | Jumps to matching case; executes until break             |
| `case value: stmt`             | Switch label                         | —                              | Constant integer value            | Jump target within switch                                |
| `default: stmt`                | Switch label                         | —                              | —                                 | Fallback target if no case matches                       |
| `fabs(x)`                      | Math function (from `<tgmath.h>`)    | `double → double`              | Double value                      | Absolute value (magnitude)                               |
| `strtod(str, endptr)`          | Convert function (from `<stdlib.h>`) | `const char*, char** → double` | String pointer, end pointer       | Converts string to double; stores end position in endptr |
| `puts(str)`                    | Output function (from `<stdio.h>`)   | `const char* → int`            | String pointer                    | Prints string followed by newline                        |
| `constexpr double eps = 1E-9;` | Computed constant (C23)              | —                              | —                                 | Compile-time constant double                             |
| `0 (false)`                    | Truth value                          | —                              | —                                 | Boolean false in scalar context                          |
| `1 (true)`                     | Truth value                          | —                              | —                                 | Boolean true in scalar context                           |
| `for (;;)`                     | Infinite loop                        | —                              | —                                 | Equivalent to `while (true)`                             |

## 3. The "Danger Zone": UB, Pitfalls, and Warnings

### 3.1 Fall-Through in switch Statements

**Problem**: Missing `break` statements cause execution to "fall through" to the next case, executing unintended code.

**Example**:

```c
switch (x) {
    case 1: printf("One\n");  // No break!
    case 2: printf("Two\n");  // Executes if x == 1
}
```

**Consequence**: Unintended code execution; logic errors.

**Mitigation**: Add `break;` after each case unless fall-through is intentional and documented.

### 3.2 Variable Definition Jump-Over in switch

**Problem**: Declaring a variable in a switch block without a case label, then jumping over it via a case label.

**Example**:

```c
switch (x) {
    unsigned tmp = 45;  // Declared here
    case 0: printf("%u\n", tmp);  // But jumped to here; tmp uninitialized
}
```

**Consequence**: Uninitialized variable access (undefined behavior).

**Mitigation**: Always use separate `{...}` blocks for each case, or declare variables after all case labels.

### 3.3 Uninitialized Loop Variables

**Problem**: Using a loop variable before explicit initialization (if not declared in `for` initialization).

**Consequence**: Reading garbage values; unpredictable loop behavior.

**Mitigation**: Always initialize loop variables; prefer declaring them in `for (init; ...)`.

### 3.4 Array Index Out of Bounds in Loops

**Problem**: Loop condition allows index to exceed array bounds (e.g., `i <= array_size` instead of `i < array_size`).

**Consequence**: Undefined behavior; reading/writing outside array storage.

**Mitigation**: Use correct loop conditions: `i < size` for zero-indexed arrays.

### 3.5 Infinite Loops and Hangs

**Problem**: Loop condition is always true or never becomes false.

**Example**: `while (1) { ... }` without a break or return statement inside.

**Consequence**: Program hangs indefinitely.

**Mitigation**: Ensure loop conditions eventually become false, or use explicit `break` statements with clear termination logic.

### 3.6 Comparison to 0, false, or true (Redundancy)

**Problem**: Explicitly comparing a scalar to 0, false, or true when the scalar itself is a valid condition.

**Example**: `if ((b != false) == true) { ... }` instead of `if (b) { ... }`.

**Consequence**: Code redundancy; reduced readability; potential type conversion issues in edge cases.

**Mitigation**: Use scalar values directly as conditions.

### 3.7 Type Mismatches in switch

**Problem**: Using a non-integer expression in a switch statement, or a non-compile-time-constant in a case label.

**Example**: `switch (x) { case x+1: ... }` (non-constant case value).

**Consequence**: Compiler error; switch doesn't work correctly.

**Mitigation**: Use integer expressions in switch; use constant values in cases.

### 3.8 Logic Errors from Floating-Point Comparisons

**Problem**: Using floating-point values in loop conditions with equality comparisons (e.g., `while (x == 1.0)`).

**Consequence**: Floating-point precision issues may cause loops to terminate unexpectedly or not at all.

**Mitigation**: For floating-point loops, use inequality comparisons with a threshold (epsilon): `while (fabs(x - target) > epsilon)`.

### 3.9 Side Effects in Loop Increments

**Problem**: Loop increment expression has complex side effects or modifies multiple variables.

**Example**: `for (size_t i = 0; i < 5; ++i, --j) { ... }` (comma operator in increment).

**Consequence**: Confusing code; potential for logic errors.

**Mitigation**: Keep loop increments simple; avoid comma operators in loop construction.

### 3.10 Negative Iteration over unsigned Types

**Problem**: Assuming unsigned loop counters can become negative or comparing against negative values.

**Example**: `for (size_t i = count; i >= 0; --i)` (if count is 0, i becomes SIZE_MAX).

**Consequence**: Loop behaves unexpectedly due to unsigned wrap-around.

**Mitigation**: Understand unsigned arithmetic; use signed types for potentially negative counters.

## 4. Comprehensive C23 Working Example

```c
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
```
