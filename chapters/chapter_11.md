# Chapter 11: Pointers

Source: 11-pointers.md

## Exhaustive Concept Breakdown

### Overview

Pointers are **the first major hurdle** to deeper C understanding but **essential for practical programming**. They enable:

- Breaking the pure function barrier (modify caller's objects)
- Dynamic data structures (linked lists, trees)
- Array access via pointer arithmetic
- Function parameters by reference (simulating pass-by-reference)

**Key insight**: Distinguish between pointer (left of arrow) and pointed-to object (right of arrow).

## 11.1 Pointer Operations

### Address-Of & Object-Of Operators

**TAKEAWAY 11.1**: A program execution that uses \* with an invalid or null pointer fails.

| Operator | Name                    | Purpose                  | Usage          |
| -------- | ----------------------- | ------------------------ | -------------- |
| `&`      | Address-of              | Get address of object    | `int* p = &x;` |
| `*`      | Object-of (dereference) | Access pointed-to object | `int y = *p;`  |

**Two meanings of `*`:**

1. **In declarations**: Creates pointer type `double* p0;`
2. **In expressions**: Dereferences pointer `*p0 = 3.14;`

**Example: Non-pure function using pointers**

```c
void double_swap(double* p0, double* p1) {
    double tmp = *p0;    /* Dereference to read */
    *p0 = *p1;           /* Dereference to write */
    *p1 = tmp;
}

double d0 = 1.0, d1 = 2.0;
double_swap(&d0, &d1);   /* Pass addresses */
/* d0 now 2.0, d1 now 1.0 */
```

### Pointer Arithmetic: Addition

**TAKEAWAY 11.2**: A valid pointer refers to the first element of an array of the reference type.
**TAKEAWAY 11.3**: The length of an array object cannot be reconstructed from a pointer.
**TAKEAWAY 11.4**: Pointers are not arrays.

**Array access via pointers:**

```c
double A[7] = { 0, 1, 2, 3, 4, 5, 6 };
double const* p = &A[0];

/* Equivalent access methods: */
double x = A[2];           /* Array indexing */
double x = *(p + 2);       /* Pointer arithmetic */
double x = p[2];           /* Pointer as array notation */
```

**Two equivalent loop patterns:**

```c
/* Method 1: Index tracking */
double sum0(size_t len, double const* a) {
    double ret = 0.0;
    for (size_t i = 0; i < len; ++i)
        ret += *(a + i);  /* a+i = address of i-th element */
    return ret;
}

/* Method 2: Pointer walking */
double sum1(size_t len, double const* a) {
    double ret = 0.0;
    for (double const* aStop = a + len; a < aStop; ++a)
        ret += *a;
    return ret;
}
```

**Critical note**: Length must be passed separately; cannot be derived from pointer.

### Pointer Arithmetic: Subtraction & Difference

**Backward traversal:**

```c
double sum_backward(size_t len, double const* a) {
    double ret = 0.0;
    double const* p = a + len - 1;  /* Start at last element */
    do {
        ret += *p;
        --p;
    } while (p > a);                 /* Continue while >= first */
    return ret;
}
```

**Pointer difference (distance between two pointers):**

```c
double const* p = &A[1];
double const* q = &A[3];
ptrdiff_t distance = q - p;  /* distance = 2 */
```

**TAKEAWAY 11.5**: Only subtract pointers to elements of the same array object.
**TAKEAWAY 11.6**: All pointer differences have type ptrdiff_t.
**TAKEAWAY 11.7**: Use ptrdiff_t to encode signed differences of positions or sizes.

## 11.2 Pointer Validity & States

### Valid Pointer States

**TAKEAWAY 11.9**: Pointers have a truth value.
**TAKEAWAY 11.10**: Set pointer variables to null as soon as you can.

| State        | Valid | Dereferenceable | Condition                                              |
| ------------ | ----- | --------------- | ------------------------------------------------------ |
| **Valid**    | ✓     | ✓               | Points to actual object                                |
| **One-past** | ✓     | ✗               | Points just beyond array (loop sentinel)               |
| **Null**     | ✓     | ✗               | Explicitly set to nullptr; evaluates false             |
| **Invalid**  | ✗     | ✗               | Uninitialized or dangling; evaluates true (dangerous!) |

**TAKEAWAY 11.11**: A program execution that accesses an object that has a non-value representation for its type fails.
**TAKEAWAY 11.12**: When dereferenced, a pointed-to object must be of the designated type.
**TAKEAWAY 11.13**: A pointer must point to a valid object, one position beyond, or be null.
**TAKEAWAY 11.14**: A program execution that computes a pointer value outside the bounds of an array object (or one element beyond) fails.

### Null Pointer Constants

**TAKEAWAY 11.15**: Use nullptr instead of NULL.

**Why nullptr over NULL:**

- NULL type under-specified in pre-C23 (could be `0`, `0L`, `(void*)0`)
- Platform-dependent; no guaranteed type
- nullptr: C23 keyword, unambiguous `nullptr_t` type

**Null pointer testing:**

```c
char const* name = nullptr;
if (name) {
    printf("Name: %s\n", name);   /* Safe if non-null */
} else {
    printf("Anonymous\n");        /* name was null */
}
```

## 11.3 Pointers & Structures

### Arrow Operator (`->`)

**TAKEAWAY 11.16**: The arrow operator provides convenient member access through pointers.

| Access          | Syntax        | Equivalent      |
| --------------- | ------------- | --------------- |
| Direct (struct) | `var.member`  | Direct member   |
| Pointer         | `ptr->member` | `(*ptr).member` |

```c
struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

struct timespec t = { 10, 500000 };
struct timespec* tp = &t;

/* Equivalent: */
long ns1 = t.tv_nsec;        /* Direct access */
long ns2 = tp->tv_nsec;      /* Pointer access */
long ns3 = (*tp).tv_nsec;    /* Explicit dereference */
```

### Wrapper Functions for Pointers to Structs

Pattern: Functions accept pointer, check for null, modify in-place, return pointer:

```c
typedef struct {
    int numerator;
    int denominator;
} Fraction;

Fraction* frac_normalize(Fraction* fp) {
    if (fp) {
        size_t g = gcd(fp->numerator, fp->denominator);
        fp->numerator /= (int)g;
        fp->denominator /= (int)g;
    }
    return fp;  /* Return for chaining */
}

/* Usage: */
Fraction f = { 12, 8 };
frac_normalize(&f);  /* f now 3/2 */
```

**Advantages:**

- Safe null handling
- Function chaining possible
- In-place modification (efficient)

### Opaque Structures

**Pattern for API encapsulation:**

```c
/* Header file (public interface) */
struct toto;  /* Forward declaration; incomplete */

struct toto* toto_create(void);      /* Allocate & initialize */
void toto_destroy(struct toto*);     /* Free resources */
void toto_process(struct toto*, int);/* Operate on object */

/* Translation unit (private implementation) */
struct toto {
    int internal_state;
    char data[100];
    /* ... all details hidden ... */
};

struct toto* toto_create(void) {
    return (struct toto*)malloc(sizeof(struct toto));
}
```

**Benefits:**

- Users cannot access internals
- Implementation can change without breaking API
- Compiler handles all struct pointers identically (same size)

**TAKEAWAY 11.17**: Pointers to structures have uniform representation regardless of struct definition.

## 11.4 Pointers & Arrays

### Array Decay to Pointer

**Implicit conversion in many contexts:**

```c
int A[10];

/* All equivalent: */
int* p1 = A;              /* Array decays to pointer */
int* p2 = &A[0];          /* Explicit first-element address */
func(A);                   /* Array parameter becomes pointer */
```

**TAKEAWAY 11.18**: Arrays decay to pointers in most contexts except sizeof and address-of.

### Safe Array Interface via Pointers

**Recommended pattern: Use array notation in function signatures**

```c
/* Preferred: clarifies intent (len elements expected) */
double sum_array(size_t len, double const a[len]);

/* Equivalent but less clear: */
double sum_array(size_t len, double const* a);
```

## 11.5 Pointers & Functions

### Function Pointers (Brief Introduction)

```c
/* Declare pointer to function returning int, taking two ints */
int (*func_ptr)(int, int);

/* Point to actual function */
int add(int a, int b) { return a + b; }
func_ptr = add;

/* Call through pointer */
int result = func_ptr(3, 4);  /* Equivalent to add(3, 4) */
```

## Syntax/Types/Functions Dictionary

### Pointer Declarations & Operations

| Construct                | Purpose                                           |
| ------------------------ | ------------------------------------------------- |
| `type* ptr;`             | Declare pointer to type                           |
| `type const* ptr;`       | Pointer to constant data                          |
| `type* const ptr;`       | Constant pointer to mutable data                  |
| `type const* const ptr;` | Constant pointer to constant data                 |
| `ptr = &obj;`            | Initialize pointer to object's address            |
| `*ptr`                   | Dereference pointer to access object              |
| `ptr + i`                | Address of i-th element in array                  |
| `ptr - q`                | Difference between two pointers (type: ptrdiff_t) |
| `ptr->member`            | Access struct member through pointer              |

### Pointer Constants

| Constant  | Type                   | Purpose                              |
| --------- | ---------------------- | ------------------------------------ |
| `nullptr` | `nullptr_t`            | Null pointer (C23)                   |
| `NULL`    | Implementation-defined | Null pointer (avoid; prefer nullptr) |

### Types

| Type        | Purpose                                                       |
| ----------- | ------------------------------------------------------------- |
| `ptrdiff_t` | Signed integer for pointer differences (include `<stddef.h>`) |
| `void*`     | Generic pointer type (for type-agnostic code)                 |

## Danger Zone: Pitfalls & Undefined Behavior

### Pointer Dereferencing Pitfalls

1. **Null pointer dereference**: `*nullptr` → segmentation fault (good; catches bugs early)
2. **Invalid pointer dereference**: Uninitialized pointer has garbage address → random memory access
3. **Dangling pointers**: Pointer to freed/deallocated object → use-after-free
4. **Out-of-bounds access**: `p + 100` on 10-element array → UB even without dereference

### Pointer Arithmetic Pitfalls

1. **Array length unknown**: Cannot derive length from pointer; must pass separately
2. **Mixing array types**: `int* p = arr; float* q = p;` reinterprets; UB
3. **Pointer subtraction outside array**: `p - q` undefined unless both in same array
4. **Integer overflow in pointer arithmetic**: Large offsets wrap or exceed bounds

### Type & Representation Pitfalls

1. **Type mismatch at dereference**: `int* p = (int*)&double_var;` accesses double as int → wrong representation
2. **Pointer to temporary**: Function returns pointer to local variable → dangling after return
3. **Casting away const**: `int* p = (int*)const_ptr;` hides const-correctness violations
4. **Opaque struct misuse**: Accessing members of forward-declared struct → compilation error (good)

### Null Pointer Pitfalls

1. **Confusing NULL vs nullptr**: NULL type varies; nullptr unambiguous
2. **Implicit null checks fail**: `if (ptr)` works; `if (ptr == NULL)` may fail due to NULL type quirks
3. **Null pointer in varargs**: `printf("%s", NULL)` undefined (must use nullptr in C23)

## Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <math.h>

/* ============================================================ */
/* 1. ADDRESS-OF & DEREFERENCE OPERATORS */
/* ============================================================ */

void demo_address_dereference(void) {
    printf("=== Address-Of & Dereference Operators ===\n");

    int value = 42;
    int* ptr = &value;  /* ptr points to value */

    printf("Value: %d\n", value);
    printf("Address: %p\n", (void*)&value);
    printf("Pointer contents: %p\n", (void*)ptr);
    printf("Dereferenced pointer: %d\n", *ptr);

    /* Modify through pointer */
    *ptr = 100;
    printf("After *ptr = 100: value = %d\n", value);
}

/* ============================================================ */
/* 2. SWAP FUNCTION (NON-PURE): MODIFYING CALLER'S OBJECTS */
/* ============================================================ */

void swap_ints(int* p0, int* p1) {
    int tmp = *p0;
    *p0 = *p1;
    *p1 = tmp;
}

void demo_swap(void) {
    printf("\n=== Swap Function (Non-Pure) ===\n");

    int a = 10, b = 20;
    printf("Before: a=%d, b=%d\n", a, b);

    swap_ints(&a, &b);  /* Pass addresses */
    printf("After swap_ints(&a, &b): a=%d, b=%d\n", a, b);
}

/* ============================================================ */
/* 3. POINTER ARITHMETIC: WALKING ARRAYS */
/* ============================================================ */

double array_sum_indexed(size_t len, const double* arr) {
    double sum = 0.0;
    for (size_t i = 0; i < len; ++i) {
        sum += *(arr + i);  /* arr + i = address of i-th element */
    }
    return sum;
}

double array_sum_walking(size_t len, const double* arr) {
    double sum = 0.0;
    const double* end = arr + len;
    for (const double* p = arr; p < end; ++p) {
        sum += *p;  /* Walk through array */
    }
    return sum;
}

double array_sum_backward(size_t len, const double* arr) {
    double sum = 0.0;
    const double* p = arr + len - 1;  /* Start at last */
    do {
        sum += *p;
        --p;
    } while (p >= arr);  /* Continue downward */
    return sum;
}

void demo_array_arithmetic(void) {
    printf("\n=== Pointer Arithmetic: Array Walking ===\n");

    double data[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    size_t count = sizeof(data) / sizeof(data[0]);

    printf("Array: ");
    for (size_t i = 0; i < count; ++i) printf("%.1f ", data[i]);
    printf("\n");

    double sum1 = array_sum_indexed(count, data);
    double sum2 = array_sum_walking(count, data);
    double sum3 = array_sum_backward(count, data);

    printf("Sum (indexed): %.1f\n", sum1);
    printf("Sum (walking forward): %.1f\n", sum2);
    printf("Sum (walking backward): %.1f\n", sum3);
}

/* ============================================================ */
/* 4. POINTER DIFFERENCE: FINDING POSITION */
/* ============================================================ */

int find_positive(size_t len, const double* arr) {
    const double* end = arr + len;

    for (const double* p = arr; p < end; ++p) {
        if (*p < 0.0) {
            ptrdiff_t pos = p - arr;  /* Distance from start */
            printf("Found negative at index %td: %.1f\n", pos, *p);
            return (int)pos;
        }
    }
    return -1;  /* Not found */
}

void demo_pointer_difference(void) {
    printf("\n=== Pointer Difference ===\n");

    double values[] = { 1.0, 2.0, -3.0, 4.0, -5.0 };
    size_t count = sizeof(values) / sizeof(values[0]);

    find_positive(count, values);
    find_positive(count, values);
}

/* ============================================================ */
/* 5. STRUCT POINTERS: ARROW OPERATOR */
/* ============================================================ */

typedef struct {
    char name[50];
    int age;
    double salary;
} Employee;

void display_employee(const Employee* emp) {
    if (!emp) {
        printf("(null employee)\n");
        return;
    }
    printf("Name: %s, Age: %d, Salary: $%.2f\n",
           emp->name, emp->age, emp->salary);
}

void increase_salary(Employee* emp, double percent) {
    if (emp) {
        emp->salary *= (1.0 + percent / 100.0);
    }
}

void demo_struct_pointers(void) {
    printf("\n=== Struct Pointers ===\n");

    Employee alice = { "Alice", 32, 75000.0 };
    Employee* eptr = &alice;

    printf("Direct access: %s, age %d\n",
           alice.name, alice.age);
    printf("Pointer access: %s, age %d\n",
           eptr->name, eptr->age);
    printf("Before raise: ");
    display_employee(eptr);

    increase_salary(eptr, 10);
    printf("After 10%% raise: ");
    display_employee(eptr);
}

/* ============================================================ */
/* 6. NULL POINTER TESTING */
/* ============================================================ */

void demo_null_pointers(void) {
    printf("\n=== Null Pointer Handling ===\n");

    int value = 42;
    int* valid_ptr = &value;
    int* null_ptr = nullptr;

    printf("Valid pointer: %s\n", valid_ptr ? "non-null" : "null");
    printf("Null pointer: %s\n", null_ptr ? "non-null" : "null");

    /* Safe null-aware access */
    if (valid_ptr) {
        printf("Dereferencing valid ptr: %d\n", *valid_ptr);
    }

    if (!null_ptr) {
        printf("Null pointer detected; skipping dereference\n");
    }
}

/* ============================================================ */
/* 7. ARRAY NOTATION WITH POINTERS */
/* ============================================================ */

void print_as_array(size_t len, int const arr[len]) {
    printf("Array (via pointer): ");
    for (size_t i = 0; i < len; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void demo_array_notation(void) {
    printf("\n=== Array Notation with Pointers ===\n");

    int nums[] = { 10, 20, 30, 40, 50 };
    size_t count = sizeof(nums) / sizeof(nums[0]);

    int* ptr = nums;  /* Decay to pointer */
    printf("Length of array: %zu\n", count);
    printf("Length via sizeof pointer: %zu\n",
           sizeof(ptr) / sizeof(int));  /* WRONG! */

    print_as_array(count, ptr);
}

/* ============================================================ */
/* 8. POINTER CHAINS: MULTIPLE LEVELS */
/* ============================================================ */

void demo_pointer_chains(void) {
    printf("\n=== Multi-Level Pointers ===\n");

    int value = 99;
    int* ptr1 = &value;           /* Pointer to int */
    int** ptr2 = &ptr1;           /* Pointer to pointer to int */

    printf("Value: %d\n", value);
    printf("Via *ptr1: %d\n", *ptr1);
    printf("Via **ptr2: %d\n", **ptr2);

    /* Modify through chain */
    **ptr2 = 200;
    printf("After **ptr2 = 200: value = %d\n", value);
}

/* ============================================================ */
/* 9. ARRAY OF POINTERS */
/* ============================================================ */

void demo_array_of_pointers(void) {
    printf("\n=== Array of Pointers ===\n");

    int a = 10, b = 20, c = 30;
    int* arr[3] = { &a, &b, &c };  /* Array of pointers */

    printf("Pointed values via array:\n");
    for (int i = 0; i < 3; ++i) {
        printf("  arr[%d] -> %d\n", i, *arr[i]);
    }
}

/* ============================================================ */
/* MAIN COORDINATION */
/* ============================================================ */

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║        Chapter 11: Pointers - C23 Comprehensive Demo      ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_address_dereference();
    demo_swap();
    demo_array_arithmetic();
    demo_pointer_difference();
    demo_struct_pointers();
    demo_null_pointers();
    demo_array_notation();
    demo_pointer_chains();
    demo_array_of_pointers();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    Demo Complete                           ║\n");
    printf("║         All pointer operations demonstrated!              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return EXIT_SUCCESS;
}
```

## Summary of Key TAKEAWAYs

1. **Address & dereference**: `&` gets address; `*` accesses pointed-to object
2. **Pointers to arrays**: Pointer = address of first element + length (passed separately)
3. **Array decay**: Arrays become pointers in most contexts
4. **Pointer arithmetic**: `p + i`, `p - q`, `p++` work like array indexing
5. **Null pointers**: Use `nullptr` (not `NULL`); always initialize
6. **Struct pointers**: Use `->` operator (not `(*p).member`)
7. **Pointer validity**: Must point to valid object, one-past, or null
8. **Array length hidden**: Cannot recover from pointer; must pass separately
9. **Opaque types**: Pointers enable API encapsulation
10. **Pure functions**: Pointers break pure property; use carefully
