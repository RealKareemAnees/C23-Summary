#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* === 4.1 Basic Allocation and Deallocation === */

void demo_malloc_free(void) {
    printf("\n--- malloc and free ---\n");

    size_t n = 5;
    int* arr = malloc(n * sizeof *arr);

    if (!arr) {
        printf("Allocation failed!\n");
        return;
    }

    for (size_t i = 0; i < n; ++i) {
        arr[i] = (int)(i * 10);
    }

    printf("Array: ");
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    // arr = NULL;  // Best practice: set to null after free
}

/* === 4.2 calloc (Zero-Initialized Allocation) === */

void demo_calloc(void) {
    printf("\n--- calloc ---\n");

    size_t n = 3;
    double* vec = calloc(n, sizeof *vec);

    if (!vec) {
        printf("Allocation failed!\n");
        return;
    }

    printf("After calloc (should be 0.0):\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  vec[%zu] = %g\n", i, vec[i]);
    }

    free(vec);
}

/* === 4.3 realloc (Resizing) === */

void demo_realloc(void) {
    printf("\n--- realloc ---\n");

    int* arr = malloc(3 * sizeof *arr);
    if (!arr) return;

    for (int i = 0; i < 3; ++i) arr[i] = i + 1;

    printf("Original (size 3): ");
    for (int i = 0; i < 3; ++i) printf("%d ", arr[i]);
    printf("\n");

    // Resize to 5 elements
    int* new_arr = realloc(arr, 5 * sizeof *arr);
    if (!new_arr) {
        printf("Realloc failed\n");
        free(arr);  // arr still valid!
        return;
    }
    arr = new_arr;

    // Initialize new elements
    arr[3] = 4;
    arr[4] = 5;

    printf("After realloc (size 5): ");
    for (int i = 0; i < 5; ++i) printf("%d ", arr[i]);
    printf("\n");

    free(arr);
}

/* === 4.4 String Duplication (C23) === */

void demo_strdup_strndup(void) {
    printf("\n--- strdup and strndup ---\n");

    const char* original = "Hello, World!";

    // Full duplication
    char* dup = strdup(original);
    if (dup) {
        printf("strdup result:  %s\n", dup);
        printf("Address original: %p\n", (void*)original);
        printf("Address dup:      %p (different!)\n", (void*)dup);
        free(dup);
    }

    // Bounded duplication
    char* dup_bounded = strndup(original, 5);
    if (dup_bounded) {
        printf("strndup(..., 5): %s\n", dup_bounded);
        free(dup_bounded);
    }
}

/* === 4.5 Circular Buffer Implementation === */

typedef struct {
    size_t start;
    size_t len;
    size_t cap;
    int* tab;
} IntCircular;

IntCircular* circ_create(size_t capacity) {
    IntCircular* c = malloc(sizeof *c);
    if (!c) return NULL;

    c->tab = malloc(capacity * sizeof *c->tab);
    if (!c->tab) {
        free(c);
        return NULL;
    }

    c->start = 0;
    c->len = 0;
    c->cap = capacity;
    return c;
}

void circ_destroy(IntCircular* c) {
    if (c) {
        free(c->tab);
        free(c);
    }
}

int circ_push(IntCircular* c, int value) {
    if (!c || c->len >= c->cap) return -1;  // Full

    size_t idx = (c->start + c->len) % c->cap;
    c->tab[idx] = value;
    c->len++;
    return 0;
}

int circ_pop(IntCircular* c) {
    if (!c || c->len == 0) return -1;  // Empty

    int val = c->tab[c->start];
    c->start = (c->start + 1) % c->cap;
    c->len--;
    return val;
}

void demo_circular_buffer(void) {
    printf("\n--- Circular Buffer ---\n");

    IntCircular* buf = circ_create(5);
    if (!buf) return;

    for (int i = 1; i <= 5; ++i) {
        circ_push(buf, i * 10);
    }
    printf("After pushing 5 elements\n");

    printf("Popping: ");
    while (buf->len > 0) {
        printf("%d ", circ_pop(buf));
    }
    printf("\n");

    circ_destroy(buf);
}

/* === 4.6 Flexible Array Member === */

typedef struct {
    size_t length;
    uint32_t data[];  // Flexible array member
} Uint32Array;

Uint32Array* uint32_array_create(size_t len) {
    size_t size = offsetof(Uint32Array, data) + len * sizeof(uint32_t);
    Uint32Array* arr = malloc(size);

    if (arr) {
        arr->length = len;
    }
    return arr;
}

void demo_flexible_array(void) {
    printf("\n--- Flexible Array Member ---\n");

    Uint32Array* arr = uint32_array_create(4);
    if (!arr) return;

    for (size_t i = 0; i < arr->length; ++i) {
        arr->data[i] = (uint32_t)(i * 100);
    }

    printf("FLA data: ");
    for (size_t i = 0; i < arr->length; ++i) {
        printf("%u ", arr->data[i]);
    }
    printf("\n");

    free(arr);
}

/* === 4.7 Storage Duration Demonstration === */

int static_var = 0;  // Static storage, initialized to 0

void demo_storage_duration(void) {
    printf("\n--- Storage Duration ---\n");

    static int call_count = 0;  // Static, initialized once
    call_count++;

    int auto_var = 42;          // Automatic (function scope)
    int* heap_ptr = malloc(sizeof *heap_ptr);

    if (heap_ptr) {
        *heap_ptr = 99;

        printf("Static (static_var): %d\n", static_var);
        printf("Static (call_count): %d (call #%d)\n", call_count, call_count);
        printf("Automatic (auto_var): %d\n", auto_var);
        printf("Heap (*heap_ptr): %d\n", *heap_ptr);

        free(heap_ptr);
    }
}

/* === 4.8 aligned_alloc === */

void demo_aligned_alloc(void) {
    printf("\n--- aligned_alloc ---\n");

    // Allocate 16-byte aligned storage
    size_t align = 16;
    double* vec = (double*)aligned_alloc(align, 4 * sizeof *vec);

    if (vec) {
        printf("Vector allocated with %zu-byte alignment\n", align);
        printf("Address: %p (aligned: %s)\n",
               (void*)vec,
               (uintptr_t)vec % align == 0 ? "YES" : "NO");

        for (int i = 0; i < 4; ++i) vec[i] = i * 1.5;
        printf("Data: ");
        for (int i = 0; i < 4; ++i) printf("%.1f ", vec[i]);
        printf("\n");

        free(vec);
    }
}

int main(void) {
    printf("=== Chapter 13: Storage ===\n");

    demo_malloc_free();
    demo_calloc();
    demo_realloc();
    demo_strdup_strndup();
    demo_circular_buffer();
    demo_flexible_array();
    demo_storage_duration();
    demo_aligned_alloc();

    printf("\n=== End Chapter 13 ===\n");
    return 0;
}
