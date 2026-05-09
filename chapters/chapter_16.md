# Chapter 16: Performance

Source: 16-performance.md

## 1. Exhaustive Concept Breakdown

### 1.1 Safety vs. Performance Trade-off

TAKEAWAY 16.1: Premature optimization is the root of all evil.

Knuth's warning emphasizes that optimizing before identifying bottlenecks wastes effort on readability and maintainability. Modern C compilers are sophisticated; most safety-first code incurs negligible overhead.

TAKEAWAY 16.2: Do not trade safety for performance.

Common risks traded for speed:

- Out-of-bounds array access
- Uninitialized objects
- Use-after-free
- Integer overflow

Safe practices that maintain performance:

1. **Initialize all block-scope variables** (eliminates 50% of uninitialized access issues).
2. **Use `calloc` instead of `malloc`** (zero-initializes, eliminates 25% more issues).
3. **Implement initialization functions** for complex dynamic structures.
4. **Use array pointer syntax with bounds**:
   - `func(double a[static 1])` – single non-null object
   - `func(double a[static 7])` – collection of exactly 7
   - `func(size_t n, double a[static n])` – collection of `n` elements (size first)
   - `func(double* a)` – single or null pointer (must handle null)
5. **Avoid taking addresses of local variables** (helps optimizer, inhibits aliasing).
6. **Use unsigned loop indices** with explicit wrap-around handling.

TAKEAWAY 16.3: Optimizers are clever enough to eliminate unused initializations.
TAKEAWAY 16.4: Different pointer argument notations result in same binary code.
TAKEAWAY 16.5: Not taking addresses of local variables helps optimizer (inhibits aliasing).

### 1.2 Inline Functions

Function call overhead (stack setup, cache misses) can slow short functions. Inlining replaces function calls with expanded code.

TAKEAWAY 16.6: Inlining can open up a lot of optimization opportunities.

**C99's `inline` keyword**:

- Enables multiple definitions in different TUs without linker errors
- Pointers to inline functions remain equal across TUs
- Unused inline functions don't bloat binary

TAKEAWAY 16.7: Adding a compatible declaration without `inline` keyword ensures symbol emission in current TU.

**Pattern** (in header file):

```c
inline unsigned square(unsigned x) { return x*x; }
// In one TU, add non-inline declaration to emit symbol:
unsigned square(unsigned x);  // Ensures symbol exists
```

### 1.3 The `restrict` Qualifier

`restrict` hints that no other pointer in scope aliases the qualified pointer, enabling aggressive optimization.

TAKEAWAY 16.8: Use `restrict` in function interfaces with pointer parameters where aliasing is known not to occur.

**Pattern**:

```c
void copy(int const* restrict src, int* restrict dst, size_t n) {
    for (size_t i = 0; i < n; ++i) dst[i] = src[i];  // Compiler assumes no overlap
}
```

If violated (src and dst overlap), behavior is undefined.

### 1.4 C23 Attributes for Optimization

**`[[unsequenced]]`**: Function has no dependencies on observable state outside parameters/locals; compilers can reorder/combine calls.

**`[[reproducible]]`**: Function produces same result given identical arguments; enables common subexpression elimination and memoization.

```c
[[unsequenced]] unsigned square(unsigned x) { return x*x; }
[[reproducible]] double sqrt_approx(double x) { return ...reproducible computation...; }
```

### 1.5 Performance Measurement

**Relevance**: Improving 100ms → 50ms in non-critical code wastes effort.

Measure using:

- Platform profilers (perf, valgrind, Intel VTune)
- Timing libraries: `<time.h>` (`clock()`, `time()`)
- Cycle counters (RDTSC on x86, `_rdtsc()` via compiler builtins)

TAKEAWAY 16.9: Always measure before and after optimization to confirm improvements.

## 2. Syntax, Types, and Functions Dictionary

### Optimization Keywords and Attributes

| Keyword/Attribute  | Purpose                                                           |
| ------------------ | ----------------------------------------------------------------- |
| `inline`           | Permits multiple definitions across TUs; hints at inlining        |
| `static`           | Internal linkage (enables static inlining in header files)        |
| `restrict`         | Promises no aliasing of qualified pointer                         |
| `register`         | Hints that variable should be in register (disables `&` operator) |
| `[[unsequenced]]`  | Function has no observable side effects; reorderable              |
| `[[reproducible]]` | Function output depends only on arguments; memoizable             |
| `alignas(T)`       | Force alignment to type T (improves cache locality)               |

### Timing Functions

| Function         | Header           | Purpose                            |
| ---------------- | ---------------- | ---------------------------------- |
| `clock()`        | `<time.h>`       | CPU time in clock ticks            |
| `time()`         | `<time.h>`       | Wall-clock seconds (coarse)        |
| `timespec_get()` | `<time.h>` (C11) | High-resolution time (nanoseconds) |

### Array Pointer Notations

| Notation                            | Meaning                               |
| ----------------------------------- | ------------------------------------- |
| `void f(int a[])`                   | Pointer to int (array decay)          |
| `void f(int a[static 1])`           | Non-null pointer to int               |
| `void f(int a[static 5])`           | Pointer to at least 5 ints            |
| `void f(size_t n, int a[static n])` | Pointer to n ints (n specified first) |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Aliasing with `restrict`

**Pitfall**: Declaring `restrict` when aliasing actually occurs.

```c
void copy_wrong(int const* restrict src, int* restrict dst) {
    for (int i = 0; i < 10; ++i) dst[i] = src[i];
}
int arr[20];
copy_wrong(&arr[0], &arr[5]);  // UB: src and dst aliases!
```

**Fix**: Only use `restrict` when you guarantee no aliasing.

### 3.2 Inline Symbol Issues

**Pitfall**: Expecting inline function to have single address across TUs.

```c
// In header:
inline int* get_static(void) { static int x; return &x; }
// Different TUs get different addresses!
```

**Fix**: Add non-inline declaration in one TU if symbol needed.

### 3.3 Premature Micro-optimization

**Pitfall**: Optimizing code before profiling; gains may be negligible.

```c
// Over-optimized, hard to read:
uint32_t x = (a+b)&0xFFFFFFFF;  // Avoid modulo
vs. uint32_t x = (uint32_t)(a+b);  // Clear intent
```

**Fix**: Profile first; optimize only hot paths.

### 3.4 Uninitialized Variables in Optimized Code

**Pitfall**: Uninitialized variable; optimizer may remove safety checks.

```c
int x;  // Uninitialized
if (x > 0) x = 10;  // Compiler may assume x is meaningful
printf("%d\n", x);  // UB: may access garbage or optimized-away check
```

**Fix**: Always initialize.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

/* === 4.1 Inline Function with Symbol Emission === */

// In header (inline definition)
inline unsigned square(unsigned x) {
    return x * x;
}

// One TU: non-inline declaration to emit symbol
unsigned square(unsigned x);

// === 4.2 Array Pointer Notations ===

void copy_safe(size_t n,
               int const src[static n],
               int dst[static n]) {
    printf("Copying %zu elements (guaranteed safe bounds)\n", n);
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i];
    }
}

void process_single(int val[static 1]) {
    printf("Processing single int: %d\n", *val);
}

/* === 4.3 Restrict Pointer (Aliasing Hint) === */

void fill_array_restrict(size_t n,
                         int restrict arr[static n]) {
    for (size_t i = 0; i < n; ++i) {
        arr[i] = (int)i * 2;
    }
}

void print_array_restrict(size_t n,
                          int const restrict arr[static n]) {
    printf("Array: ");
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* === 4.4 Pure Function Attributes (C23) === */

[[unsequenced]] unsigned absolute_diff(int a, int b) {
    return (unsigned)(a > b ? a - b : b - a);
}

[[reproducible]] double square_root_approx(double x) {
    if (x <= 0.0) return 0.0;
    double guess = x / 2.0;
    for (int i = 0; i < 5; ++i) {
        guess = (guess + x / guess) / 2.0;
    }
    return guess;
}

/* === 4.5 Performance Measurement === */

void measure_function_cost(void) {
    printf("\n--- Performance Measurement ---\n");

    clock_t start = clock();

    // Dummy computation
    volatile uint64_t sum = 0;
    for (uint64_t i = 0; i < 1000000; ++i) {
        sum += i * i;
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Operation time: %f seconds\n", elapsed);
    printf("Result: %llu (volatile to prevent optimization)\n", sum);
}

/* === 4.6 Alignment for Performance === */

alignas(64) struct CacheAligned {
    int data[16];  // 64-byte cache line alignment
};

void demo_alignment(void) {
    printf("\n--- Alignment for Cache Locality ---\n");

    struct CacheAligned ca = {};

    printf("CacheAligned address: %p\n", (void*)&ca);
    printf("Aligned to 64-byte boundary: %s\n",
           ((uintptr_t)&ca) % 64 == 0 ? "YES" : "NO");
}

/* === 4.7 Safe Initialization (Maintains Performance) === */

typedef struct {
    int * restrict data;
    size_t capacity;
    size_t length;
} DynArray;

DynArray array_create(size_t capacity) {
    DynArray arr = {0};  // Safe initialization (optimized away)

    arr.data = (int *)calloc(capacity, sizeof *arr.data);
    if (arr.data) {
        arr.capacity = capacity;
        arr.length = 0;
    }

    return arr;
}

void array_push(DynArray * restrict arr, int value) {
    if (arr && arr->length < arr->capacity) {
        arr->data[arr->length++] = value;
    }
}

void array_destroy(DynArray * restrict arr) {
    if (arr) {
        free(arr->data);
        *arr = (DynArray){0};  // Safe reset
    }
}

/* === 4.8 Inline Function Optimization === */

inline double fused_multiply_add(double a, double b, double c) {
    return a * b + c;
}

void demo_inlining(void) {
    printf("\n--- Inline Function Demonstration ---\n");

    double results[5];
    for (int i = 0; i < 5; ++i) {
        results[i] = fused_multiply_add(2.0 * i, 3.0, 1.5);
    }

    for (int i = 0; i < 5; ++i) {
        printf("Result[%d]: %f\n", i, results[i]);
    }
}

int main(void) {
    printf("=== Chapter 16: Performance ===\n");

    printf("\n--- Inline and Array Pointers ---\n");
    printf("square(7) = %u\n", square(7));

    int src[5] = { 1, 2, 3, 4, 5 };
    int dst[5] = {0};
    copy_safe(5, src, dst);

    int val = 42;
    process_single(&val);

    printf("\n--- Restrict Pointers ---\n");
    fill_array_restrict(5, dst);
    print_array_restrict(5, dst);

    printf("\n--- Pure Function Attributes ---\n");
    printf("absolute_diff(10, 3) = %u\n", absolute_diff(10, 3));
    printf("sqrt_approx(4.0) = %f\n", square_root_approx(4.0));

    measure_function_cost();
    demo_alignment();
    demo_inlining();

    printf("\n--- Dynamic Array (Safe + Performant) ---\n");
    DynArray arr = array_create(10);
    for (int i = 0; i < 5; ++i) {
        array_push(&arr, i * 10);
    }
    printf("Array length: %zu\n", arr.length);
    array_destroy(&arr);

    printf("\n=== End Chapter 16 ===\n");
    return 0;
}
```

**Key Takeaways**:

- Safety-first code (initialization, bounds) maintains performance; compilers optimize away overhead.
- Inline functions enable cross-TU code expansion and optimization.
- `restrict` hints allow aggressive aliasing-free optimization.
- C23 attributes (`[[unsequenced]]`, `[[reproducible]]`) enable compiler optimizations.
- Always profile before optimizing; most code is non-critical.
