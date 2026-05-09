# Chapter 13: Storage

Source: 13-storage.md

## 1. Exhaustive Concept Breakdown

### 1.1 Dynamic Memory Allocation

Dynamic allocation creates storage for objects with lifetimes not bound to block scope. Five allocation functions in `<stdlib.h>`:

- **`malloc(size)`**: Allocates `size` bytes, returns uninitialized `void*` or null.
- **`free(ptr)`**: Deallocates; accepts null (no-op) and only valid malloc/calloc/realloc/aligned_alloc pointers.
- **`calloc(nmemb, size)`**: Allocates `nmemb * size` bytes, initializes all bits to 0, returns `void*` or null.
- **`realloc(ptr, size)`**: Resizes existing allocation; may relocate; preserves initial part on enlarge, returns new pointer or null.
- **`aligned_alloc(alignment, size)`**: Allocates with specified alignment, returns `void*` or null.

TAKEAWAY 13.1: Only use allocation functions with size strictly greater than zero.
TAKEAWAY 13.2: Failed allocations result in a null pointer.
TAKEAWAY 13.3: Prefer `strndup` over `strdup` (strndup accepts length bound).
TAKEAWAY 13.4: Don't cast the return of malloc (implicit conversion works; cast hides missing headers).
TAKEAWAY 13.5: Storage allocated through malloc is uninitialized and has no type.

Two POSIX functions (imported in C23) simplify string allocation:

```c
char* strdup(char const s[static 1]);      // malloc + strlen + strcpy
char* strndup(char const s[static 1], size_t n);  // malloc + bounded copy
```

**Idiom**: Use `sizeof *pointer` for malloc size to maintain consistency if type changes.

```c
double* vec = malloc(length * sizeof *vec);  // Not: sizeof(double)
```

### 1.2 Allocation Consistency and Memory Leaks

TAKEAWAY 13.6: malloc indicates failure by returning a null pointer.
TAKEAWAY 13.7: For every allocation, there must be a free.
TAKEAWAY 13.8: For every free, there must be malloc/calloc/aligned_alloc/realloc (but realloc combines dealloc+alloc).
TAKEAWAY 13.9: Only call free with pointers returned by malloc family or null (never on local variables or partial object pointers).

**Memory leak**: Unreleased allocated objects exhaust heap resources, causing crashes or performance degradation.

### 1.3 Complete Dynamic Allocation Example: Circular Buffer

A circular buffer dynamically allocates space for a fixed-capacity queue:

```c
struct circular {
    size_t start;           // First element index
    size_t len;             // Number of elements
    size_t cap;             // Maximum capacity
    double* tab;            // Dynamically allocated array
};
```

Initialization allocates `tab`:

```c
circular* c_init(circular* c, size_t cap) {
    if (c) {
        if (cap) {
            *c = (circular){ .cap = cap, .tab = malloc(sizeof(double[cap])) };
            if (!c->tab) c->cap = 0;  // Mark failure
        } else {
            *c = (circular){ };  // Zero-init
        }
    }
    return c;
}
```

Resizing handles wrap-around via `realloc` and `memmove`:

```c
circular* c_resize(circular* c, size_t nlen) {
    // Enlarge via realloc; handle wrap-around by copying portions
    double* ntab = realloc(c->tab, sizeof(double[nlen]));
    if (!ntab) return nullptr;  // Preserve old on failure
    // Adjust wrap-around data...
    c->tab = ntab;
    c->cap = nlen;
    return c;
}
```

TAKEAWAY 13.10: A structure with flexible array member must have enough storage to access the structure itself.
TAKEAWAY 13.11: Consistency between length member and flexible array member must be maintained manually.

### 1.4 Flexible Array Members (FLA)

Flexible array members couple array data directly to struct metadata:

```c
struct ua32 {
    size_t length;
    uint32_t data[];  // Flexible array member (must be last)
};

size_t len = 32;
size_t size = offsetof(ua32, data) + sizeof(uint32_t[len]);
ua32* ap = calloc(size, 1);
ap->length = len;
for (size_t i = 0; i < ap->length; ++i) {
    printf("%u\n", ap->data[i]);
}
free(ap);
```

FLAs avoid pointer indirection and keep data/metadata coherent but require manual size tracking.

### 1.5 Storage Duration and Object Lifetime

Objects have **storage duration** (when storage is reserved) and **identifier visibility** (when name is in scope):

TAKEAWAY 13.12: Identifiers only have visibility inside their scope, starting at declaration.
TAKEAWAY 13.13: Identifier visibility can be shadowed by same name in subordinate scope.
TAKEAWAY 13.14: Every definition of a variable creates a new, distinct object.

**Storage durations**:

- **Automatic**: Variables in functions; lifetime ends at block exit.
- **Static**: File scope or `static` keyword; lifetime is entire program.
- **Allocated**: `malloc`/`calloc`/etc.; lifetime from allocation to `free()`.
- **Thread**: Thread-specific storage; lifetime is thread execution.

### 1.6 Object Literals and Shadowing

TAKEAWAY 13.15: Read-only object literals may overlap (compiler optimization).

```c
char const A[] = { 'a', 'b', 'c', '\0' };  // Distinct object
char const B[] = { 'a', 'b', 'c', '\0' };  // May or may not be same object
char const* c = "abc";    // String literal
char const* d = "abc";    // May or may not overlap with c
```

String literals and const compound literals are read-only and may be merged by compilers.

## 2. Syntax, Types, and Functions Dictionary

### Allocation Functions

| Function                     | Header             | Returns         | Purpose                           |
| ---------------------------- | ------------------ | --------------- | --------------------------------- |
| `malloc(size)`               | `<stdlib.h>`       | `void*` or null | Allocate uninitialized storage    |
| `calloc(n, size)`            | `<stdlib.h>`       | `void*` or null | Allocate zero-initialized storage |
| `realloc(ptr, size)`         | `<stdlib.h>`       | `void*` or null | Resize allocation (may relocate)  |
| `aligned_alloc(align, size)` | `<stdlib.h>`       | `void*` or null | Allocate with specified alignment |
| `free(ptr)`                  | `<stdlib.h>`       | void            | Deallocate (accepts null)         |
| `strdup(s)`                  | `<string.h>` (C23) | `char*` or null | Allocate and copy string          |
| `strndup(s, n)`              | `<string.h>` (C23) | `char*` or null | Allocate and copy bounded string  |

### Keywords and Macros

| Keyword/Macro            | Purpose                                                |
| ------------------------ | ------------------------------------------------------ |
| `static`                 | Static storage duration (lifetime = program)           |
| `auto`                   | Automatic storage duration (default for locals)        |
| `register`               | Automatic duration with no address (optimization hint) |
| `extern`                 | External linkage; defer definition to another TU       |
| `offsetof(type, member)` | Byte offset of struct member (operator, not function)  |
| `[[nodiscard]]`          | Attribute: warn if return value ignored                |

### Types

| Type        | Purpose                                         |
| ----------- | ----------------------------------------------- |
| `size_t`    | Unsigned integer for sizes (result of `sizeof`) |
| `ptrdiff_t` | Signed integer for pointer differences          |
| `mbstate_t` | Multibyte character parsing state               |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Memory Leaks

**Pitfall**: Unreleased allocations exhaust heap.

```c
void leak(void) {
    int* p = malloc(100 * sizeof *p);
    do_something();
    return;  // p never freed!
}
```

**Fix**: Always pair `malloc` with `free`.

### 3.2 Double-Free and Use-After-Free

**Pitfall**: Calling `free()` twice on same pointer or using after `free()`.

```c
int* p = malloc(sizeof *p);
free(p);
free(p);        // CRASH: double-free
int val = *p;   // CRASH: use-after-free
```

**Fix**: Set freed pointers to null; check before reuse.

### 3.3 Invalid free() Arguments

**Pitfall**: Calling `free()` on non-malloc pointers or partial objects.

```c
int arr[10];
free(arr);           // CRASH: local variable
free(&arr[5]);       // CRASH: partial object
int* p = malloc(100);
free(p + 10);        // CRASH: not malloc return value
```

**Fix**: Only `free()` pointers returned by malloc/calloc/realloc/aligned_alloc or null.

### 3.4 Buffer Overrun in Realloc

**Pitfall**: Forgetting that `realloc()` may return a different pointer.

```c
int* p = malloc(10 * sizeof *p);
p = realloc(p, 20 * sizeof *p);  // CORRECT
// Saving old pointer for error recovery: also correct
int* new_p = realloc(p, 20 * sizeof *p);
if (!new_p) {  // p still valid if realloc fails!
    free(p);
}
```

**Fix**: Check return value; preserve original pointer if realloc fails.

### 3.5 FLA Storage Violations

**Pitfall**: Not allocating enough space for FLA structure.

```c
struct ua32* ap = malloc(sizeof(struct ua32));  // WRONG: no space for data[]
ap->data[0] = 42;  // CRASH: out-of-bounds write
```

**Fix**: Use `offsetof(struct_type, member) + array_size` for total allocation.

## 4. Comprehensive C23 Working Example

```c
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
```

**Key Takeaways from Example**:

- `malloc` allocates uninitialized storage; `calloc` zero-initializes.
- Always check allocation success; handle null returns.
- `realloc` may relocate; save return value separately to preserve original pointer on failure.
- `strdup`/`strndup` simplify string copying with automatic allocation.
- Circular buffers demonstrate practical allocation + structure management.
- Flexible array members avoid indirection; require explicit size tracking.
- Static variables persist across function calls; automatic variables have block scope.
