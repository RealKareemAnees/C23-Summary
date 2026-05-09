# Chapter 8: C Library Functions

Source: 08-librraryfunctions.md

## Exhaustive Concept Breakdown

### Overview

The C standard library separates into two categories:

1. **Platform abstraction layer**: Functions abstracting platform-specific operations (I/O, system calls)
2. **Basic tools**: Portable, frequently-used functionalities (strtod, printf, memory operations)

**TAKEAWAY 8.1**: Failure is always an option.
**TAKEAWAY 8.2**: Check the return value of library functions for errors.
**TAKEAWAY 8.3**: Fail fast, fail early, and fail often.

## 8.1 C Library Headers & Organization

### Standard Headers (Partial List)

| Header          | Category       | Purpose                                              |
| --------------- | -------------- | ---------------------------------------------------- |
| `<assert.h>`    | Debugging      | Runtime condition assertions                         |
| `<complex.h>`   | Types          | Complex number support                               |
| `<ctype.h>`     | String         | Character classification (isalpha, isdigit, etc.)    |
| `<errno.h>`     | Errors         | Error code tracking                                  |
| `<fenv.h>`      | Floating-point | Floating-point environment control                   |
| `<float.h>`     | Types          | Floating-point type properties                       |
| `<inttypes.h>`  | Types          | Formatted conversion (integer types)                 |
| `<iso646.h>`    | Operators      | Alternative operator spellings                       |
| `<limits.h>`    | Types          | Integer type properties                              |
| `<locale.h>`    | I/O            | Internationalization/localization                    |
| `<math.h>`      | Math           | Mathematical functions (sin, cos, sqrt, etc.)        |
| `<stdarg.h>`    | Functions      | Variable-argument function support                   |
| `<stdatomic.h>` | Threads        | Atomic operations                                    |
| `<stdbit.h>`    | Math           | Bit operations (C23 new)                             |
| `<stdbool.h>`   | Types          | Boolean type support                                 |
| `<stdckdint.h>` | Math           | Checked integer arithmetic (C23 new)                 |
| `<stddef.h>`    | Types          | size_t, ptrdiff_t, nullptr                           |
| `<stdint.h>`    | Types          | Exact-width integer types                            |
| `<stdio.h>`     | I/O            | File I/O (printf, scanf, file operations)            |
| `<stdlib.h>`    | General        | Core functions (exit, malloc, strtod, abs, div)      |
| `<string.h>`    | Strings        | String manipulation (strlen, strcpy, strcmp, memcpy) |
| `<tgmath.h>`    | Math           | Type-generic mathematical functions                  |
| `<threads.h>`   | Threads        | Thread creation/synchronization (C11+)               |
| `<time.h>`      | Time           | Time/date functions (localtime, strftime)            |
| `<wchar.h>`     | Strings        | Wide character strings                               |

## 8.2 Error Handling Strategies

### Error Return Conventions

**Table 8.2: Error Return Strategies**

| Failure Signal       | Test            | When Used                 | Example                                           |
| -------------------- | --------------- | ------------------------- | ------------------------------------------------- |
| Null pointer         | `!value`        | Other return values valid | fopen returns NULL on error                       |
| Special error code   | `value == EOF`  | Other values valid        | puts returns EOF on failure                       |
| Nonzero value        | `value != 0`    | Value otherwise unneeded  | fgetpos returns non-0 on error                    |
| Special success code | `value != code` | Distinguish failure case  | thrd_create returns error code                    |
| Negative value       | `value < 0`     | Positive = counter        | printf returns -1 on error, byte count on success |

### Standard Error Handling Pattern

```c
if (puts("output") == EOF) {
    perror("output failed:");  /* Prints diagnostic based on errno */
    exit(EXIT_FAILURE);        /* Fail immediately */
}
```

### errno: Global Error State

- **Purpose**: Track errors across library calls
- **Type**: Global variable (implementation detail)
- **Limitation**: Only one error at a time
- **Reset**: Must manually set `errno = 0;` if recovery needed
- **Perror**: Prints human-readable error message

```c
void puts_safe(char const s[static 1]) {
    static bool failed = false;
    if (!failed && puts(s) == EOF) {
        perror("output failed:");
        failed = true;
        errno = 0;  /* Reset for potential recovery */
    }
}
```

**TAKEAWAY 8.4**: Identifier names terminating with \_s are reserved.

## 8.3 Bounds-Checking Interfaces (Annex K, Optional)

### Purpose

Many C library functions vulnerable to buffer overflow (non-portable security enhancement).

### Activation

```c
#if !__STDC_LIB_EXT1__
# error "Bounds checking not available"
#endif
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
/* Use printf_s from here on */
```

### Conventions

- **\_s suffix**: `printf_s`, `strcpy_s`, etc. (replaces unsafe versions)
- **Constraint violations**: Runtime failures with diagnostic output
- **Status**: Optional; many platforms deliberately omit (controversial design)

**TAKEAWAY 8.4**: Identifier names terminating with \_s are reserved.

## 8.4 Platform Preconditions via Preprocessor

### Checking Feature Availability

**TAKEAWAY 8.5**: Missed preconditions for the execution platform must abort compilation.
**TAKEAWAY 8.6**: In a preprocessor conditional, only evaluate macros and integer literals.
**TAKEAWAY 8.7**: In a preprocessor conditional, unknown identifiers evaluate to 0.

### Preprocessor Conditional Operators

| Operator                  | Argument       | Purpose                            |
| ------------------------- | -------------- | ---------------------------------- |
| `defined(X)`              | Macro name     | Tests if macro defined             |
| `__has_include(header)`   | Header name    | Tests if header available (C23)    |
| `__has_embed(file)`       | Binary file    | Tests if file embeddable (C23)     |
| `__has_c_attribute(attr)` | Attribute name | Tests if attribute supported (C23) |

### Shortcut Syntax

| Shortcut       | Equivalent          | Availability |
| -------------- | ------------------- | ------------ |
| `#ifdef(X)`    | `#if defined(X)`    | Traditional  |
| `#ifndef(X)`   | `#if !defined(X)`   | Traditional  |
| `#elifdef(X)`  | `#elif defined(X)`  | C23+         |
| `#elifndef(X)` | `#elif !defined(X)` | C23+         |

### Static Assertions (Compile-Time)

```c
static_assert(sizeof(double) == sizeof(long double),
    "Extra precision needed for convergence.");
```

- **Timing**: Evaluated at compile-time (after preprocessing)
- **Pre-C23**: `_Static_assert` keyword
- **C23+**: `static_assert` keyword

## 8.5 Integer Arithmetic Functions

### Basic Functions

| Function                       | Purpose                  | Header       |
| ------------------------------ | ------------------------ | ------------ |
| `int abs(int x)`               | Absolute value           | `<stdlib.h>` |
| `long labs(long x)`            | Long absolute value      | `<stdlib.h>` |
| `long long llabs(long long x)` | Long long absolute value | `<stdlib.h>` |

### Division with Remainder

```c
struct div_t result = div(x, y);
/* result.quot = x / y (quotient) */
/* result.rem = x % y (remainder) */
```

### Checked Integer Arithmetic (C23, `<stdckdint.h>`)

```c
unsigned result = 0;
bool overflow = ckd_add(&result, UINT_MAX, UINT_MAX);
/* overflow = true; result = UINT_MAX - 1 (modular reduction) */
```

**Functions**: `ckd_add`, `ckd_sub`, `ckd_mul`

- **Return**: Boolean (true = overflow, false = no overflow)
- **Parameter**: Pointer to result location
- **Guarantee**: Unconditionally valid; result holds least significant bits

### Bit Operations (C23, `<stdbit.h>`)

| Function                 | Aspect | Purpose                                  |
| ------------------------ | ------ | ---------------------------------------- |
| `stdc_bit_width(x)`      | Width  | Number of bits needed to represent x     |
| `stdc_leading_zeros(x)`  | Count  | Leading zero bits                        |
| `stdc_trailing_zeros(x)` | Count  | Trailing zero bits                       |
| `stdc_popcount(x)`       | Count  | Number of 1-bits                         |
| `stdc_has_single_bit(x)` | Test   | True if exactly one bit set (power of 2) |

**Properties**: All functions have defined results for all unsigned inputs (no undefined behavior).

## 8.6 Mathematical Functions

### Using `<tgmath.h>` (Type-Generic Macros)

- **Advantage**: Dispatches to appropriate function based on argument type
- **Usage**: `sin(x)` automatically calls `sinf()`, `sin()`, or `sinl()` per type of x
- **Prefer**: Type-generic versions over explicit type suffixes

### Common Categories

| Category        | Examples                              | Header                    |
| --------------- | ------------------------------------- | ------------------------- |
| Trigonometric   | sin, cos, tan, asin, acos, atan       | `<math.h>` / `<tgmath.h>` |
| Hyperbolic      | sinh, cosh, tanh, asinh, acosh, atanh | `<math.h>` / `<tgmath.h>` |
| Exponential/Log | exp, log, log2, log10, pow            | `<math.h>` / `<tgmath.h>` |
| Roots/Magnitude | sqrt, cbrt, hypot, fabs               | `<math.h>` / `<tgmath.h>` |
| Rounding        | ceil, floor, round, trunc, nearbyint  | `<math.h>` / `<tgmath.h>` |
| Classification  | isnan, isinf, isfinite, isnormal      | `<math.h>` (macros)       |

### Quality Guarantee

- **High precision**: Modern implementations correctly rounded
- **Don't reimplement**: Use library versions (optimized, tested)

## 8.7 Time Handling

### Time Representation

```c
struct tm {
    int tm_sec;     /* Seconds [0, 60] */
    int tm_min;     /* Minutes [0, 59] */
    int tm_hour;    /* Hours [0, 23] */
    int tm_mday;    /* Day of month [1, 31] */
    int tm_mon;     /* Months since January [0, 11] */
    int tm_year;    /* Years since 1900 */
    int tm_wday;    /* Days since Sunday [0, 6] */
    int tm_yday;    /* Days since January 1 [0, 365] */
    int tm_isdst;   /* Daylight Saving Time flag */
};

struct timespec {
    time_t tv_sec;      /* Seconds (≥ 0) */
    long tv_nsec;       /* Nanoseconds [0, 999999999] */
};
```

### Common Functions

- `time(NULL)` → current time_t
- `localtime(&time_t)` → struct tm (local time)
- `gmtime(&time_t)` → struct tm (GMT)
- `mktime(&struct tm)` → time_t (convert struct to time)
- `strftime(buffer, size, format, &struct tm)` → formatted string

## 8.8 Program Control & Termination

### Program Termination Functions

```c
[[noreturn]] void exit(int status);        /* Normal exit */
[[noreturn]] void _Exit(int status);       /* Immediate exit (no cleanup) */
[[noreturn]] void quick_exit(int status);  /* Quick exit (fast) */
[[noreturn]] void abort(void);             /* Abnormal termination */
```

### Program Assertion

```c
#include <assert.h>
assert(a > 0);  /* If false: prints diagnostic and calls abort() */
```

- **Disabling**: Compile with `-DNDEBUG` to disable assertions
- **Usage**: Check preconditions, invariants, postconditions
- **Not for error handling**: Use explicit checks where needed

## Syntax/Types/Functions Dictionary

### Error Checking

| Pattern              | Purpose                  |
| -------------------- | ------------------------ |
| `if (!ptr)`          | Test null pointer return |
| `if (val == EOF)`    | Test EOF error code      |
| `if (val < 0)`       | Test negative error code |
| `perror("context:")` | Print error diagnostic   |
| `errno = 0;`         | Clear error state        |

### Preprocessor Conditionals

| Construct                       | Purpose                |
| ------------------------------- | ---------------------- |
| `#if defined(X)` / `#ifdef(X)`  | Test macro defined     |
| `#if __has_include(<header.h>)` | Test header available  |
| `static_assert(cond, "msg")`    | Compile-time assertion |
| `#error "message"`              | Compilation failure    |
| `#warning "message"`            | Compilation warning    |

### Integer Arithmetic

| Function                             | Signature              |
| ------------------------------------ | ---------------------- |
| `int abs(int)`                       | Absolute value         |
| `struct div_t div(int, int)`         | Quotient and remainder |
| `bool ckd_add(int* r, int a, int b)` | Checked addition (C23) |

### Math Functions (via `<tgmath.h>`)

| Function                   | Purpose                       |
| -------------------------- | ----------------------------- |
| `sin(x), cos(x), tan(x)`   | Trigonometric                 |
| `exp(x), log(x), pow(x,y)` | Exponential/logarithm         |
| `sqrt(x), cbrt(x)`         | Root functions                |
| `isnan(x), isinf(x)`       | Floating-point classification |

### Time Functions

| Function                      | Purpose                    |
| ----------------------------- | -------------------------- |
| `time(NULL)`                  | Get current time           |
| `localtime(&t)`               | Convert to local struct tm |
| `strftime(buf, sz, fmt, &tm)` | Format time as string      |

## Danger Zone: Pitfalls & Undefined Behavior

### Error Handling Pitfalls

1. **Ignoring return values**: "It won't fail" → crashes in production
2. **Inconsistent error checks**: Testing `== 0` for one function, `< 0` for another
3. **errno not reset**: Old error state causes false positives
4. **errno not thread-local**: Multithreaded programs see interleaved errors

### Bounds-Checking Pitfalls

1. **Mixing \_s and non-\_s versions**: Weak protection if used inconsistently
2. **False security**: \_s functions not universally available (controversial)
3. **Performance cost**: Checking adds overhead (acceptable tradeoff)

### Preprocessor Pitfalls

1. **Unknown identifiers → 0**: `#if MY_MACRO` with undefined MY_MACRO is falsy (silent)
2. **Missing guards**: Including header twice without `#include` guards
3. **Macro expansion surprises**: `ckd_add` may expand code multiple times; store in temp
4. **Type mismatches in checked arithmetic**: Signed/unsigned mixing leads to surprising overflow detection

### Mathematical Function Pitfalls

1. **NaN results**: `sqrt(-1.0)` returns NaN (not error); must check explicitly
2. **Overflow to infinity**: `exp(1000.0)` → INFINITY (not portable error)
3. **Precision loss**: Floating-point operations accumulate rounding error
4. **Don't reimplement**: Custom math functions rarely match library precision

### Time Function Pitfalls

1. **Year 2038 problem**: 32-bit time_t overflows in 2038 (use 64-bit on modern systems)
2. **struct tm inconsistency**: Uninitialized tm_wday/tm_yday fields inconsistent with actual date
3. **Leap second handling**: tm_sec can be 60; rare but causes bugs
4. **Timezone complexity**: DST changes, historical variations cause subtle issues

## Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdckdint.h>
#include <stdbit.h>
#include <ctype.h>
#include <errno.h>

/* ============================================================ */
/* 1. ERROR HANDLING PATTERNS */
/* ============================================================ */

void demo_error_handling(void) {
    printf("=== Error Handling Strategies ===\n");

    /* Pattern 1: Null pointer return */
    FILE* f = fopen("/nonexistent", "r");
    if (!f) {
        perror("fopen failed:");
        printf("(This is expected; continuing...)\n");
    } else {
        fclose(f);
    }

    /* Pattern 2: Special error code (EOF) */
    const char* test_str = "hello";
    int result = puts(test_str);
    if (result == EOF) {
        perror("puts failed");
    } else {
        printf("(puts returned %d, success)\n", result);
    }
}

/* ============================================================ */
/* 2. PREPROCESSOR CONDITIONALS & COMPILE-TIME CHECKS */
/* ============================================================ */

void demo_preprocessor_checks(void) {
    printf("\n=== Preprocessor Conditionals ===\n");

    #ifdef __STDC_VERSION__
    printf("C Standard Version: ");
    if (__STDC_VERSION__ == 202311L) {
        printf("C23\n");
    } else if (__STDC_VERSION__ == 201710L) {
        printf("C17\n");
    } else {
        printf("Unknown (%ld)\n", __STDC_VERSION__);
    }
    #else
    printf("Non-standard C (no __STDC_VERSION__)\n");
    #endif

    /* Static assertion at compile time */
    static_assert(sizeof(long) >= sizeof(int),
        "long must be at least as large as int");
    printf("(Static assertions passed)\n");
}

/* ============================================================ */
/* 3. INTEGER ARITHMETIC FUNCTIONS */
/* ============================================================ */

void demo_integer_arithmetic(void) {
    printf("\n=== Integer Arithmetic Functions ===\n");

    /* abs / labs / llabs */
    int neg = -42;
    printf("abs(%d) = %d\n", neg, abs(neg));

    /* div: quotient and remainder */
    struct div_t d = div(17, 5);
    printf("div(17, 5): quotient=%d, remainder=%d\n", d.quot, d.rem);
}

/* ============================================================ */
/* 4. CHECKED INTEGER ARITHMETIC (C23) */
/* ============================================================ */

void demo_checked_arithmetic(void) {
    printf("\n=== Checked Integer Arithmetic (C23) ===\n");

    unsigned int a = UINT_MAX;
    unsigned int b = 5;
    unsigned int result = 0;

    bool overflow = ckd_add(&result, a, b);
    printf("ckd_add(%u, %u):\n", a, b);
    printf("  Overflow: %s\n", overflow ? "YES" : "NO");
    printf("  Result (modular): %u\n", result);
}

/* ============================================================ */
/* 5. BIT MANIPULATION (C23) */
/* ============================================================ */

void demo_bit_operations(void) {
    printf("\n=== Bit Operations (C23) ===\n");

    unsigned int value = 0b101010;  /* 42 in binary */

    printf("Value: %u (binary: 0b101010)\n", value);
    printf("  stdc_bit_width: %d bits needed\n",
           stdc_bit_width(value));
    printf("  stdc_leading_zeros: %d zeros\n",
           stdc_leading_zeros(value));
    printf("  stdc_trailing_zeros: %d zeros\n",
           stdc_trailing_zeros(value));
    printf("  stdc_popcount: %d one-bits\n",
           stdc_popcount(value));
    printf("  stdc_has_single_bit: %s (power of 2?)\n",
           stdc_has_single_bit(value) ? "YES" : "NO");
}

/* ============================================================ */
/* 6. MATHEMATICAL FUNCTIONS */
/* ============================================================ */

void demo_math_functions(void) {
    printf("\n=== Mathematical Functions (via <tgmath.h>) ===\n");

    /* Trigonometric */
    double angle = M_PI / 6;  /* 30 degrees */
    printf("sin(π/6) = %.4f\n", sin(angle));
    printf("cos(π/6) = %.4f\n", cos(angle));
    printf("tan(π/6) = %.4f\n", tan(angle));

    /* Exponential and logarithm */
    printf("\nexp(1) = %.4f (e)\n", exp(1.0));
    printf("log(e) = %.4f\n", log(M_E));
    printf("log10(100) = %.4f\n", log10(100.0));

    /* Power and root */
    printf("\nsqrt(16) = %.4f\n", sqrt(16.0));
    printf("pow(2, 8) = %.4f\n", pow(2.0, 8.0));
    printf("cbrt(27) = %.4f\n", cbrt(27.0));

    /* Classification */
    double special1 = 0.0 / 0.0;  /* NaN */
    double special2 = 1.0 / 0.0;  /* Infinity */
    printf("\n0/0 is NaN: %s\n", isnan(special1) ? "YES" : "NO");
    printf("1/0 is infinity: %s\n", isinf(special2) ? "YES" : "NO");
    printf("42.0 is finite: %s\n", isfinite(42.0) ? "YES" : "NO");
}

/* ============================================================ */
/* 7. TIME FUNCTIONS */
/* ============================================================ */

void demo_time_functions(void) {
    printf("\n=== Time Functions ===\n");

    /* Get current time */
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);

    printf("Current local time:\n");
    printf("  Year: %d\n", local_time->tm_year + 1900);
    printf("  Month: %d (0=January)\n", local_time->tm_mon);
    printf("  Day: %d\n", local_time->tm_mday);
    printf("  Hour: %d\n", local_time->tm_hour);
    printf("  Minute: %d\n", local_time->tm_min);
    printf("  Second: %d\n", local_time->tm_sec);

    /* Format as string */
    char buffer[256];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    printf("  Formatted: %s\n", buffer);
}

/* ============================================================ */
/* 8. CHARACTER CLASSIFICATION */
/* ============================================================ */

void demo_char_classification(void) {
    printf("\n=== Character Classification (<ctype.h>) ===\n");

    const char* test = "Hello123!@";
    printf("String: \"%s\"\n\n", test);

    for (size_t i = 0; i < strlen(test); ++i) {
        char c = test[i];
        printf("'%c':", c);
        if (isalpha(c)) printf(" alphabetic");
        if (isdigit(c)) printf(" digit");
        if (isspace(c)) printf(" whitespace");
        if (isupper(c)) printf(" uppercase");
        if (islower(c)) printf(" lowercase");
        if (ispunct(c)) printf(" punctuation");
        printf("\n");
    }
}

/* ============================================================ */
/* 9. PROGRAM TERMINATION FUNCTIONS */
/* ============================================================ */

void demo_termination_functions(void) {
    printf("\n=== Program Termination Control ===\n");
    printf("(Not actually calling exit/abort; just demonstrating)\n");
    printf("Available: exit(), _Exit(), quick_exit(), abort()\n");
    printf("All are [[noreturn]]: never return to caller\n");
}

/* ============================================================ */
/* 10. ASSERTIONS FOR PRECONDITIONS */
/* ============================================================ */

int safe_divide(int a, int b) {
    assert(b != 0);  /* Precondition */
    return a / b;
}

void demo_assertions(void) {
    printf("\n=== Assertions for Preconditions ===\n");

    printf("safe_divide(10, 2) = %d\n", safe_divide(10, 2));

    printf("(Not calling safe_divide(10, 0) to avoid assertion failure)\n");
}

/* ============================================================ */
/* MAIN COORDINATION */
/* ============================================================ */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Chapter 8: C Library Functions - C23 Comprehensive Demo  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_error_handling();
    demo_preprocessor_checks();
    demo_integer_arithmetic();
    demo_checked_arithmetic();
    demo_bit_operations();
    demo_math_functions();
    demo_time_functions();
    demo_char_classification();
    demo_termination_functions();
    demo_assertions();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║      All library functions executed successfully!         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
```

## Summary of Key TAKEAWAYs

1. **Always check errors**: Library failure is always possible
2. **Fail immediately**: Don't defer error handling
3. **Error codes vary**: Each function has its own error convention
4. **errno is global**: Reset when needed; thread-unsafe
5. **Bounds-checking optional**: Annex K not universal
6. **Preprocessor conditionals**: Test feature availability at compile-time
7. **Checked arithmetic**: Use C23 ckd_add, ckd_sub, ckd_mul for overflow detection
8. **Math functions**: Use library versions (correctly rounded, optimized)
9. **Time complexity**: Year 2038 problem; struct tm range limitations
10. **Assertions for preconditions**: Use assert() to validate assumptions
