# Chapter 4: Expressing Computations

Source: 04-expressingcomputations.md

> Expressions compute values through operators applied to operands. C distinguishes value operators (arithmetic, bitwise, logical), object operators (assignment, increment/decrement), and type operators (sizeof, alignof). Understanding operator precedence, evaluation order guarantees (short-circuit semantics for &&, ||, ?:), and the distinction between side effects and pure computation is essential for writing correct, predictable expressions.

## 1. Exhaustive Concept Breakdown

### 1.1 Operands and Operators Fundamentals

An **operator** is a symbol or keyword that performs a computation on one or more **operands** (the values/objects the operator acts upon). Operands are expressions or variables providing input to the operator.

**Classification of operators**:

- **Value operators**: Act on values; produce values (arithmetic: +, -, \*, /; bitwise: &, |, ^)
- **Object operators**: Act on objects (variables); modify objects or return modified values (assignment: =, +=, -=; increment/decrement: ++, --)
- **Type operators**: Return information about types (sizeof, alignof)

### 1.2 The size_t Type and VALUE RANGES

The `size_t` type represents unsigned integer values suitable for sizes, counts, and array indices. Its range is [0, SIZE_MAX], where SIZE_MAX is the maximum representable value.

**Platform variation**:

- Minimal requirement: SIZE_MAX ≥ 2^15 - 1 (65,535) - rare on modern systems
- Common on 32-bit systems: SIZE_MAX = 2^32 - 1 (4,294,967,295)
- Common on 64-bit systems: SIZE_MAX = 2^64 - 1 (18,446,744,073,709,551,615)

**TAKEAWAY 4.1**: size_t represents values in range [0, SIZE_MAX]

**Important property**: size_t is an unsigned type; all arithmetic on unsigned types is mathematically well-defined (modulo arithmetic).

### 1.3 Unsigned Integer Arithmetic

Unsigned types guarantee predictable arithmetic behavior through modulo semantics:

**TAKEAWAY 4.2**: Unsigned arithmetic is always well defined  
**TAKEAWAY 4.3**: Operations +, -, \* on size_t provide mathematically correct results if representable; overflow wraps modulo (SIZE_MAX + 1)  
**TAKEAWAY 4.6**: Arithmetic on size_t implicitly computes modulo (SIZE_MAX + 1)  
**TAKEAWAY 4.7**: On overflow, unsigned arithmetic wraps around (SIZE_MAX + 1 == 0, 0 - 1 == SIZE_MAX)

Division and remainder operations:
**TAKEAWAY 4.4**: For unsigned values, `a == (a/b)*b + (a%b)` (fundamental property)  
**TAKEAWAY 4.5**: Unsigned / and % are only well defined if the second operand is not 0  
**TAKEAWAY 4.8**: The result of unsigned / and % is always smaller than the operands  
**TAKEAWAY 4.9**: Unsigned / and % cannot overflow

### 1.4 Arithmetic Operators: +, -, \*, /, %

**Binary arithmetic operators** (+, -, \*, /): Combine two operands; results follow standard arithmetic rules for the operand types.

**Unary operators**: -a (negation: produces negative), +a (identity: produces same value)

**Examples with size_t**:

- `45 - 7 = 38` (subtraction works for unsigned as long as result ≥ 0)
- `(45 - 7) * 2 = 76` (subtraction followed by multiplication)
- Parentheses enforce operator precedence: `45 - 7 * 2 = 31` (multiplication before subtraction)

**Division and remainder** (/ and %):

- Integer division: `a / b` = number of times `b` fits into `a` (truncated, not rounded)
- Remainder: `a % b` = leftover after extracting maximum `b`s from `a`
- Example: `14 / 3 = 4`, `14 % 3 = 2`; verify: `4 * 3 + 2 = 14` ✓

**Practical use**: `%` with 12 computes time on 12-hour clock; `%` with 60 extracts minute component.

### 1.5 Objects vs. Values: lvalue and rvalue

An **lvalue** (addressable value) is an expression referring to an object in memory that can be assigned to (left side of assignment).  
An **rvalue** (right value) is an expression producing a value (right side of assignment).

**Example**: `a = 42`

- `a` is an lvalue (a variable object)
- `42` is an rvalue (a literal value)

**Key distinction**: Assignment modifies objects; arithmetic operates on values.

### 1.6 Assignment and Compound Assignment Operators

**Simple assignment**: `a = value` stores value in object a.

**Compound assignment operators**: `@=` combine an operator and assignment:

- `a += b` ≡ `a = a + b`
- `a -= b` ≡ `a = a - b`
- `a *= b` ≡ `a = a * b`
- `a /= b` ≡ `a = a / b`
- `a %= b` ≡ `a = a % b`

**Syntax requirement**: Operator characters must be directly attached (no spaces within the operator).

**TAKEAWAY 4.10**: Operators must have all characters directly attached

### 1.7 Increment and Decrement Operators

**Prefix forms** (++a, --a): Modify the variable, then return the new value.  
**Postfix forms** (a++, a--): Return the old value, then modify the variable.

**Equivalence**:

- `++i` ≡ `i += 1` (prefix)
- `i++` ≡ `i += 1` but returns old value (postfix)

**Performance consideration**: Prefix may be marginally more efficient (no temporary needed) but rarely matters in modern code.

### 1.8 Side Effects and Expression Purity

A **side effect** is a modification to program state beyond the primary purpose of an expression (modifying variables, performing I/O).

**TAKEAWAY 4.11**: Side effects in value expressions are evil  
**TAKEAWAY 4.12**: Never modify more than one object in a statement

**Problematic example**: `a = b = c += ++d;` (cascading modifications)

**Better practice**: Separate assignments and modifications into discrete statements.

### 1.9 Comparison Operators

**Comparison operators** (==, !=, <, >, <=, >=) compare two values and return 0 (false) or 1 (true).

**TAKEAWAY 4.13**: Comparison operators return false (0) or true (1)

**Creative use**: Comparison results (0 or 1) can be used as array indices:

```c
int sign[2] = {0, 0};
for (...) {
    sign[(array[i] < 1.0)] += 1;  // Increment sign[1] if true, sign[0] if false
}
```

### 1.10 Boolean Logic Operators

**NOT operator** (!a): Negates a scalar value; returns 0 if a is non-zero, 1 if a is zero.

**AND operator** (&&a && b): Logical AND; returns 1 only if both operands are true (non-zero).

**OR operator** (||): Logical OR; returns 1 if at least one operand is true (non-zero).

**TAKEAWAY 4.14**: Logic operators return false (0) or true (1)

**Short-circuit evaluation**:
**TAKEAWAY 4.15**: &&, ||, ?:, and , evaluate their first operand first

- `&&` does not evaluate the right operand if the left is false (0)
- `||` does not evaluate the right operand if the left is true (1)
- Example: `if (b != 0 && (a/b) > 1)` safely avoids division by zero

### 1.11 The Ternary Conditional Operator

The **ternary operator** (? :) provides conditional expression evaluation: `condition ? expr_true : expr_false`

**Behavior**: Evaluates condition; if true, evaluates and returns expr_true; if false, evaluates and returns expr_false. Only the selected branch is evaluated (short-circuit).

**Example**: `min = (a < b) ? a : b;`

**Use case**: Safe computation with domain restrictions:

```c
double sqrt_result = (x < 0) ? CMPLX(0, sqrt(-x)) : CMPLX(sqrt(x), 0);
// Avoids passing negative value to sqrt()
```

### 1.12 Evaluation Order and Operator Sequencing

**TAKEAWAY 4.17**: Most operators don't sequence their operands  
**TAKEAWAY 4.18**: Function calls don't sequence their argument expressions

Rule: The order in which operands are evaluated is **unspecified for most operators**. This is a source of subtle bugs.

**Examples of undefined order**:

- `f(a) + g(b)`: Which function is called first? Unknown.
- `printf("%g and %g\n", f(a), f(b))`: Unknown argument evaluation order.

**Consequence**: If f or g modify variables that the other reads, behavior is unpredictable.

**Solution**:
**TAKEAWAY 4.19**: Functions within expressions should not have side effects

### 1.13 The Comma Operator

The **comma operator** (,) evaluates operands left-to-right and returns the value of the right operand.

**TAKEAWAY 4.16**: Don't use the , operator

**Trap**: `A[i, j]` is NOT a 2D index; it's equivalent to `A[j]` (comma operator returns right operand).

### 1.14 Bitwise Operators (Advanced Topics)

Bitwise operators (&, |, ^, ~, <<, >>) operate on individual bits of integers. Covered in detail in later chapters; briefly mentioned in operator tables.

### 1.15 sizeof and alignof Type Operators

**sizeof(type) or sizeof(object)**: Returns the size in bytes of a type or object as a value of type size_t.

**alignof(type) or alignof(object)**: Returns the byte-alignment requirement of a type.

## 2. Syntax, Types & Functions Dictionary

| Entity          | Type/Category        | Signature                   | Parameters                       | Return                                        |
| --------------- | -------------------- | --------------------------- | -------------------------------- | --------------------------------------------- |
| `a + b`         | Binary addition      | size_t, size_t → size_t     | Operands                         | Sum (with wrapping on overflow)               |
| `a - b`         | Binary subtraction   | size_t, size_t → size_t     | Operands                         | Difference (unsigned arithmetic)              |
| `a * b`         | Multiplication       | size_t, size_t → size_t     | Operands                         | Product (with wrapping on overflow)           |
| `a / b`         | Integer division     | size_t, size_t → size_t     | Dividend, divisor (must be ≠ 0)  | Quotient                                      |
| `a % b`         | Remainder            | size_t, size_t → size_t     | Dividend, divisor (must be ≠ 0)  | Remainder                                     |
| `-a`            | Unary negation       | size_t → size_t             | Value                            | Negation (wraps for unsigned)                 |
| `+a`            | Unary plus           | size_t → size_t             | Value                            | Identity (same value)                         |
| `a = b`         | Assignment           | lvalue, rvalue → rvalue     | Object (lvalue), value (rvalue)  | Value assigned                                |
| `a += b`        | Compound addition    | lvalue, rvalue → rvalue     | Object, value                    | a = a + b; returns new value                  |
| `a -= b`        | Compound subtraction | lvalue, rvalue → rvalue     | Object, value                    | a = a - b; returns new value                  |
| `++a`           | Prefix increment     | lvalue → rvalue             | Variable                         | Increments, returns new value                 |
| `a++`           | Postfix increment    | lvalue → rvalue             | Variable                         | Returns old value, then increments            |
| `a == b`        | Equality             | scalar, scalar → int        | Operands                         | 1 if equal; 0 if not equal                    |
| `a != b`        | Inequality           | scalar, scalar → int        | Operands                         | 0 if equal; 1 if not equal                    |
| `a < b`         | Less than            | scalar, scalar → int        | Operands                         | 1 if a<b; 0 otherwise                         |
| `a > b`         | Greater than         | scalar, scalar → int        | Operands                         | 1 if a>b; 0 otherwise                         |
| `a <= b`        | Less or equal        | scalar, scalar → int        | Operands                         | 1 if a≤b; 0 otherwise                         |
| `a >= b`        | Greater or equal     | scalar, scalar → int        | Operands                         | 1 if a≥b; 0 otherwise                         |
| `!a`            | Logical NOT          | scalar → int                | Value                            | 1 if a is 0; 0 if a is non-zero               |
| `a && b`        | Logical AND          | scalar, scalar → int        | Left operand, right (if needed)  | 1 if both true; 0 otherwise (short-circuit)   |
| `a \|\| b`      | Logical OR           | scalar, scalar → int        | Left operand, right (if needed)  | 1 if either true; 0 otherwise (short-circuit) |
| `a ? b : c`     | Ternary conditional  | scalar, expr, expr → varies | Condition, true expr, false expr | Value of selected branch (short-circuit)      |
| `a , b`         | Comma operator       | any, any → type of b        | Operands                         | Value of right operand                        |
| `sizeof(type)`  | Size operator        | type → size_t               | Type or object                   | Byte size                                     |
| `alignof(type)` | Alignment operator   | type → size_t               | Type or object                   | Byte alignment                                |

## 3. The "Danger Zone": UB, Pitfalls, and Warnings

### 3.1 Division by Zero

**Problem**: Dividing by zero (/ or %) with denominator = 0.

**Consequence**: **Undefined Behavior**; program may crash, return garbage, or behave unpredictably.

**Mitigation**: Always check denominator ≠ 0 before division.

### 3.2 Arithmetic Overflow (Signed vs. Unsigned)

**Problem**: For unsigned types, overflow is well-defined (wrapping). For signed types, overflow is undefined.

**Consequence**: For unsigned: predictable wrap-around. For signed: anything can happen (crashes, wrong values).

**Mitigation**: Use unsigned types when overflow is expected; validate ranges for signed types.

### 3.3 Evaluation Order Unpredictability

**Problem**: Relying on a specific evaluation order for operands when no order is guaranteed.

**Example**: `f(a) + g(b)` where f and g have side effects.

**Consequence**: Behavior depends on compiler, optimization level, and phase of the moon; non-portable.

**Mitigation**: Avoid side effects in expressions; separate side-effect statements.

### 3.4 Short-Circuit Evaluation and Hidden Logic

**Problem**: Misunderstanding when the second operand of && or || is NOT evaluated.

**Example**: `if (ptr != NULL && ptr->field > 10) {...}` properly checks ptr before dereferencing.

**Advantage**: Short-circuit prevents accessing ptr->field if ptr is NULL.

**Pitfall**: If a desired side effect is in the second operand, it may not execute.

### 3.5 Side Effects in Aggregate Expressions

**Problem**: Modifying multiple variables in a single expression.

**Example**: `a = b = c += ++d;`

**Consequence**: Code is hard to understand; evaluation order ambiguities make it unreliable.

**Mitigation**: Use simple, one-object-per-statement style.

### 3.6 The Comma Operator Trap

**Problem**: Confusing A[i, j] with multi-dimensional array indexing (it's actually A[j] via comma operator).

**Consequence**: Logic error; wrong array element accessed.

**Mitigation**: Never use comma operator; use explicit 2D indexing via flattened indices or proper 2D data structures.

### 3.7 Type Mismatches in Assignments

**Problem**: Assigning a value of one type to a variable of incompatible type.

**Example**: `unsigned char c = 1000;` (1000 exceeds 255, the max for unsigned char).

**Consequence**: Implicit conversion; truncation; loss of data.

**Mitigation**: Ensure type compatibility; use explicit casts when conversion is intentional.

### 3.8 Prefix vs. Postfix Increment Confusion

**Problem**: Misunderstanding the return value difference between ++a and a++.

**Example**: `int x = 5; int y = x++; // y = 5, x = 6`

**Consequence**: Off-by-one errors in logic; unexpected variable values.

**Mitigation**: Understand semantics; prefer prefix (marginally more efficient).

### 3.9 Operator Precedence Mistakes

**Problem**: Relying on implicit precedence rather than explicit parentheses.

**Example**: `a + b * c` (multiplication before addition per precedence).

**Consequence**: If programmer intended addition first, logic error.

**Mitigation**: Use parentheses to enforce intended precedence; don't rely on memorized precedence tables.

### 3.10 Modulo with Negative Operands (Signed Types)

**Problem**: Behavior of % with negative signed integers is implementation-defined in C89; well-defined in C99+.

**Consequence**: Non-portable code; unexpected results on older compilers.

**Mitigation**: Understand modulo semantics for your target standard; use unsigned types when possible.

## 4. Comprehensive C23 Working Example

```c
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
```
