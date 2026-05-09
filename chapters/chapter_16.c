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
