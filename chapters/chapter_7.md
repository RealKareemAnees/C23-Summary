# Chapter 7: Functions

Source: 07-functions.md

## Exhaustive Concept Breakdown

### Overview

Functions provide **modularity**, **code reuse**, and **clear interfaces**. They allow unconditional transfer of control (unlike conditional statements), enable algorithm implementation, and establish pre/post-conditions via parameters and return types.

**Key Motivations:**

- Avoid code repetition and copy-paste errors
- Reduce compilation time (compile once)
- Enable future reuse in unforeseen contexts
- Provide clear data flow interfaces
- Support stack-based algorithm formulation

## 7.1 Simple Functions & Prototypes

### Function Declaration vs Definition

- **Declaration**: `returnType functionName(paramType param1, paramType param2);` (prototype)
- **Definition**: Includes body with `{ /* implementation */ }`
- **Implicit conversion**: Compiler converts arguments and return values per prototype

```c
extern double fbar(double);     /* Declaration (prototype) */
double result = fbar(2) / 2;    /* 2 (int) → 2.0 (double); result float-divided */
```

**TAKEAWAY 7.1**: All functions must have prototypes.
**Exception**: Variable-argument functions like `printf` (use `<stdarg.h>`; avoid in general)

### Return Values & Control Flow

- **Multiple returns**: Function can have several return statements; all must be consistent with declared type
- **Type conversion**: Return value converted to declared return type automatically
- **void functions**: Can omit return or use `return;` without expression
- **Implicit return**: Reaching end of function body = implicit `return;` for void functions only

**TAKEAWAY 7.2**: Functions have only one entry but can have several returns.
**TAKEAWAY 7.3**: A function return must be consistent with its type.
**TAKEAWAY 7.4**: Reaching the end of the body of a non-void function would return uninitialized value (UB).
**TAKEAWAY 7.5**: Reaching the end of the body of a function is only allowed for void functions.

## 7.2 The main Function: Special Entry Point

### Standard Prototypes (Must Implement One)

```c
int main(void);                           /* No arguments */
int main(int argc, char* argv[argc+1]);   /* Command-line arguments */
```

**Platform-specific variations** (non-portable):

- `void main(void);` on embedded platforms
- Third parameter for environment on some systems

**TAKEAWAY 7.6**: Use EXIT_SUCCESS and EXIT_FAILURE as return values for main.

### Return Value Semantics

- **EXIT_SUCCESS** (0): Program executed successfully
- **EXIT_FAILURE** (non-zero, typically 1): Program failed
- **Only these two values** are guaranteed portable across platforms

**TAKEAWAY 7.7**: Reaching the end of main is equivalent to a return with EXIT_SUCCESS.

### The exit function

```c
[[noreturn]] void exit(int status);
```

- **Behavior**: Terminates program execution immediately; never returns
- **Parameter**: Status code (EXIT_SUCCESS or EXIT_FAILURE)
- **Equivalence**: `exit(s)` ≡ `return s;` from main
- **Attribute**: `[[noreturn]]` indicates never returns to caller

**TAKEAWAY 7.8**: Calling exit(s) is equivalent to the evaluation of returns in main.
**TAKEAWAY 7.9**: exit never fails and never returns to its caller.

### Command-Line Arguments (argv)

**TAKEAWAY 7.10**: All command-line arguments are transferred as strings.
**TAKEAWAY 7.11**: argv[0] points to the name of the program invocation.
**TAKEAWAY 7.12**: argv[argc] is a null pointer.

```c
int main(int argc, char* argv[argc+1]) {
    for (int i = 0; i < argc; ++i) {
        printf("Arg %d: %s\n", i, argv[i]);
    }
    /* argv[argc] == nullptr (terminator) */
}
```

**Conversion example**:

```c
double value = strtod(argv[1], nullptr);  /* Parse argv[1] as double */
```

## 7.3 Recursion: Functions Calling Themselves

### Recursive Function Essentials

**TAKEAWAY 7.13**: Make all preconditions for a function explicit.
**TAKEAWAY 7.14**: In a recursive function, first check the termination condition.

### Termination Condition

- **Mandatory**: Every recursive function must have a base case that doesn't recurse
- **Default termination**: Check if recursion should stop (e.g., `if (!a) return b;`)
- **Missing termination**: Infinite recursion → stack exhaustion → program crash

### Wrapper Functions

- **Preconditions**: Ensure all assumptions before recursive calls
- **Implementation**: Separate wrapper function enforces preconditions once

**TAKEAWAY 7.15**: Ensure the preconditions of a recursive function in a wrapper function.

**Example: Euclid's Algorithm for GCD**

```c
inline size_t gcd2(size_t a, size_t b) {
    assert(a <= b);           /* Precondition */
    if (!a) return b;         /* Base case */
    size_t r = b % a;
    return gcd2(r, a);        /* Recursive call with guaranteed a <= b */
}

inline size_t gcd(size_t a, size_t b) {
    assert(a); assert(b);     /* Wrapper: enforce preconditions */
    if (a < b) return gcd2(a, b);
    else return gcd2(b, a);
}
```

### Multiple Recursion: Fibonacci Example

```c
size_t fib(size_t n) {
    if (n < 3) return 1;           /* Base case */
    return fib(n-1) + fib(n-2);    /* Two recursive calls */
}
```

**Cost**: Exponential time complexity; repeats same computations

- Formula: `T(n) ≈ C₁ * φⁿ` where φ ≈ 1.618 (golden ratio)
- **TAKEAWAY 7.16**: Multiple recursion may lead to exponential computation times.

### Algorithm Improvement: Memoization (Caching)

```c
size_t fibCacheRec(size_t n, size_t cache[static n]) {
    if (!cache[n-1]) {
        cache[n-1] = fibCacheRec(n-1, cache) + fibCacheRec(n-2, cache);
    }
    return cache[n-1];
}
```

- **Result**: Linear time complexity `T(n) = C₂ * n`
- **Tradeoff**: Storage (array) → computation time reduction

**TAKEAWAY 7.17**: A bad algorithm will never lead to a performing implementation.
**TAKEAWAY 7.18**: Improving an algorithm can dramatically improve performance.

### Iterative Alternatives

```c
void fib2rec(size_t n, size_t buf[static 2]) {
    if (n > 2) {
        size_t res = buf[0] + buf[1];
        buf[1] = buf[0];
        buf[0] = res;
        fib2rec(n-1, buf);  /* Tail recursion: only 2-element buffer */
    }
}
```

- **Advantage**: Constant space O(1) vs O(n) for memoization
- **Tail recursion**: Can be optimized to iteration by compiler

## Syntax/Types/Functions Dictionary

### Function Declaration & Definition

| Construct                                      | Purpose                            |
| ---------------------------------------------- | ---------------------------------- |
| `returnType name(paramType p1, paramType p2);` | Function declaration (prototype)   |
| `returnType name(/* params */) { /* body */ }` | Function definition                |
| `void name(void);`                             | Function with no parameters        |
| `void name(void);`                             | Function with no return value      |
| `[[noreturn]] void exit(int);`                 | Never-returning function attribute |

### Special Functions

| Function | Signature                                                    | Purpose                              |
| -------- | ------------------------------------------------------------ | ------------------------------------ |
| `main`   | `int main(void)` or `int main(int argc, char* argv[argc+1])` | Program entry point                  |
| `exit`   | `[[noreturn]] void exit(int status)`                         | Terminate program                    |
| `strtod` | `double strtod(const char* s, char** endptr)`                | Parse string to double               |
| `assert` | `void assert(condition)`                                     | Check precondition (aborts if false) |

### Control Flow Keywords

| Keyword        | Purpose                                  |
| -------------- | ---------------------------------------- |
| `return;`      | Return from function (no value for void) |
| `return expr;` | Return value from function               |
| `EXIT_SUCCESS` | Macro: successful program termination    |
| `EXIT_FAILURE` | Macro: failed program termination        |

## Danger Zone: Pitfalls & Undefined Behavior

### Function Declaration Pitfalls

1. **Missing prototype**: Pre-C23 allowed implicit declarations; now mandatory
2. **Inconsistent signatures**: Declaration ≠ definition causes subtle bugs
3. **Type conversion surprises**: `fbar(2)` converts `int` to `double`, may lose precision

### Recursion Pitfalls

1. **Missing or unreachable base case**: Infinite recursion → stack overflow
2. **Wrong precondition check**: Recursive call violates assumptions
3. **Exponential algorithms**: Naive Fibonacci recomputes same values repeatedly
4. **Stack depth limits**: Deep recursion exhausts stack space (~10k calls typically)

### main & Program Termination Pitfalls

1. **Wrong return type**: Some platforms allow `void main(void)` (non-portable)
2. **Non-standard prototypes**: Third environment parameter varies by platform
3. **Missing EXIT_SUCCESS/EXIT_FAILURE**: Using arbitrary integers is non-portable
4. **exit() in signal handlers**: Can cause re-entrancy issues

### Argument Passing Pitfalls

1. **All arguments are strings**: Even numeric argv[] are text; must parse with strtod, atoi, etc.
2. **argv[0] platform-dependent**: May be full path or just executable name
3. **Missing argc bounds check**: Accessing argv without checking argc causes UB
4. **Variable-argument functions**: printf format specifier mismatches cause UB (avoid varargs)

## Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================================================ */
/* 1. SIMPLE FUNCTIONS WITH PROTOTYPES */
/* ============================================================ */

/* Forward declarations (prototypes) */
int add(int a, int b);
int multiply(int a, int b);
void print_menu(void);

/* Function definitions */
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

void print_menu(void) {
    printf("=== Simple Functions Demo ===\n");
    printf("Sum: %d\n", add(5, 3));
    printf("Product: %d\n", multiply(5, 3));
}

/* ============================================================ */
/* 2. RECURSION: GCD (Greatest Common Divisor) */
/* ============================================================ */

/* Recursive implementation with preconditions */
inline size_t gcd2(size_t a, size_t b) {
    assert(a <= b);         /* Precondition: a must be ≤ b */
    if (!a) return b;       /* Base case */
    size_t r = b % a;
    return gcd2(r, a);      /* Recursive: (b%a, a) always satisfies condition */
}

/* Wrapper: enforces preconditions */
inline size_t gcd(size_t a, size_t b) {
    assert(a && b);         /* Both must be non-zero */
    if (a < b) return gcd2(a, b);
    else return gcd2(b, a);
}

void demo_gcd(void) {
    printf("\n=== GCD Recursion (Euclid's Algorithm) ===\n");
    printf("GCD(18, 30) = %zu\n", gcd(18, 30));
    printf("GCD(100, 35) = %zu\n", gcd(100, 35));
    printf("GCD(17, 19) = %zu (coprime)\n", gcd(17, 19));
}

/* ============================================================ */
/* 3. MULTIPLE RECURSION: EXPONENTIAL TIME (NAIVE) */
/* ============================================================ */

size_t fib_naive(size_t n) {
    if (n < 3) return 1;
    return fib_naive(n-1) + fib_naive(n-2);
}

/* ============================================================ */
/* 4. OPTIMIZED RECURSION: LINEAR TIME (MEMOIZATION) */
/* ============================================================ */

size_t fib_cache_rec(size_t n, size_t cache[static n]) {
    if (!cache[n-1]) {
        cache[n-1] = fib_cache_rec(n-1, cache) +
                     fib_cache_rec(n-2, cache);
    }
    return cache[n-1];
}

size_t fib_cache(size_t n) {
    if (n+1 <= 3) return 1;

    size_t cache[n];
    for (size_t i = 0; i < n; ++i) cache[i] = 0;

    cache[0] = 1;
    cache[1] = 1;

    return fib_cache_rec(n, cache);
}

/* ============================================================ */
/* 5. ITERATIVE ALTERNATIVE: TAIL RECURSION OPTIMIZATION */
/* ============================================================ */

void fib2_rec(size_t n, size_t buf[static 2]) {
    if (n > 2) {
        size_t res = buf[0] + buf[1];
        buf[1] = buf[0];
        buf[0] = res;
        fib2_rec(n-1, buf);
    }
}

size_t fib2(size_t n) {
    size_t buf[2] = { 1, 1 };
    fib2_rec(n, buf);
    return buf[0];
}

void demo_fibonacci(void) {
    printf("\n=== Fibonacci: Exponential vs Optimized ===\n");

    size_t n = 15;
    printf("Naïve fib(%zu) = %zu (very slow)\n", n, fib_naive(n));
    printf("Cached fib(%zu) = %zu (linear time)\n", n, fib_cache(n));
    printf("Tail-recursive fib(%zu) = %zu (constant space)\n", n, fib2(n));

    printf("\nSequence (tail-recursive):\n");
    for (size_t i = 1; i <= 10; ++i) {
        printf("fib(%zu) = %zu\n", i, fib2(i));
    }
}

/* ============================================================ */
/* 6. COMMAND-LINE ARGUMENT PROCESSING */
/* ============================================================ */

void process_arguments(int argc, char* argv[argc+1]) {
    printf("\n=== Command-Line Arguments ===\n");
    printf("Program name (argv[0]): %s\n", argv[0]);
    printf("Total arguments (argc): %d\n", argc);
    printf("Remaining arguments:\n");

    for (int i = 1; i < argc; ++i) {
        printf("  argv[%d] = \"%s\"\n", i, argv[i]);
    }

    /* Demonstrate argv[argc] == NULL */
    printf("argv[argc] == NULL: %s\n",
           (argv[argc] == NULL) ? "true" : "false");
}

/* ============================================================ */
/* 7. FUNCTION THAT RETURNS DIFFERENT PATHS */
/* ============================================================ */

int validate_age(int age) {
    if (age < 0) {
        fprintf(stderr, "Error: negative age\n");
        return -1;      /* Error path */
    }
    if (age < 18) {
        return 0;       /* Minor */
    }
    if (age < 65) {
        return 1;       /* Adult */
    }
    return 2;           /* Senior */
}

void demo_multi_return(void) {
    printf("\n=== Multiple Returns ===\n");

    int ages[] = { 5, 18, 35, 70, -1 };
    const char* status[] = { "Minor", "Adult", "Senior", "Error" };

    for (size_t i = 0; i < 5; ++i) {
        int result = validate_age(ages[i]);
        if (result < 0) {
            printf("Age %d: Invalid (error)\n", ages[i]);
        } else {
            printf("Age %d: %s\n", ages[i], status[result]);
        }
    }
}

/* ============================================================ */
/* 8. HELPER FUNCTIONS WITH PRECONDITIONS */
/* ============================================================ */

struct Fraction {
    int numerator;
    int denominator;
};

struct Fraction reduce_fraction(struct Fraction f) {
    /* Precondition: denominator ≠ 0 */
    assert(f.denominator != 0);

    size_t g = gcd(
        (size_t)((f.numerator < 0) ? -f.numerator : f.numerator),
        (size_t)f.denominator
    );

    return (struct Fraction) {
        .numerator = f.numerator / (int)g,
        .denominator = f.denominator / (int)g
    };
}

void demo_fractions(void) {
    printf("\n=== Fractions with Preconditions ===\n");

    struct Fraction fracs[] = {
        { 12, 8 },
        { 15, 25 },
        { 7, 1 }
    };

    for (size_t i = 0; i < 3; ++i) {
        struct Fraction reduced = reduce_fraction(fracs[i]);
        printf("%d/%d → %d/%d\n",
               fracs[i].numerator, fracs[i].denominator,
               reduced.numerator, reduced.denominator);
    }
}

/* ============================================================ */
/* MAIN: COORDINATING ALL DEMOS */
/* ============================================================ */

int main(int argc, char* argv[argc+1]) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║         Chapter 7: Functions - C23 Comprehensive Demo     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    print_menu();
    demo_gcd();
    demo_fibonacci();
    demo_multi_return();
    demo_fractions();

    /* Echo command-line arguments if provided */
    if (argc > 1) {
        process_arguments(argc, argv);
    } else {
        printf("\n(Run with arguments to see argc/argv demo: %s arg1 arg2 ...)\n", argv[0]);
    }

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║         All functions executed successfully!              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;  /* TAKEAWAY 7.7: Explicit EXIT_SUCCESS */
}
```

## Summary of Key TAKEAWAYs

1. **Prototypes**: All functions must have prototypes visible at call site
2. **main Special**: Two standard forms; return EXIT_SUCCESS/EXIT_FAILURE only
3. **Recursion**: Base case mandatory; wrapper functions enforce preconditions
4. **Algorithm matters**: Naive recursion exponential; memoization linear
5. **Arguments**: All command-line args are strings; must parse explicitly
6. **Multiple returns**: Allowed; all must match declared return type
7. **Pass by value**: All parameters passed by value (pointers for reference semantics)
