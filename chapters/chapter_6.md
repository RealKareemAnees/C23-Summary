# Chapter 6: Derived Data Types

Source: 06-deriveddatatypes.md

## Exhaustive Concept Breakdown

### Overview

All C types beyond the 18 base types are **derived types** created through four strategies: **arrays**, **pointers**, **structures**, and **unions**. A fifth mechanism, **typedef**, creates type aliases (not new types).

## 6.1 Arrays: Grouping Homogeneous Objects

### Array Declaration & Structure

- **Declaration syntax**: `type name[N];` combines N objects of identical type into one aggregate
- **Multidimensional arrays**: `double C[M][N];` declares M arrays of type `double[N]`, read right-to-left
- **Array element access**: `a[i]` accesses the i-th element; `C[i][j]` is equivalent to `(C[i])[j]`
- **Designated initializers**: `type a[] = { [5] = val, ... };` specify exact positions

**TAKEAWAY 6.1**: Arrays are not pointers.
**TAKEAWAY 6.2**: An array in a condition evaluates to true (via array decay).
**TAKEAWAY 6.3**: There are array objects but no array values.
**TAKEAWAY 6.4**: Arrays can't be compared.
**TAKEAWAY 6.5**: Arrays can't be assigned to.

### Array Length & Types

- **Constant-length arrays (CLAs)**: Length known at compile-time via ICE (integer constant expression)
- **Variable-length arrays (VLAs)**: Length computed at runtime; restrictions apply (no initialization, function-local only, optional in C23)
- **FLAs (fixed-length arrays)**: CLAs with no restrictions; VLAs are optional
- **Length computation**: `sizeof(A) / sizeof(A[0])` yields array length

**TAKEAWAY 6.6**: VLAs only can have default initializers.
**TAKEAWAY 6.7**: VLAs can't be declared outside functions.
**TAKEAWAY 6.8**: The length of an FLA is determined by an integer constant expression or by an initializer.
**TAKEAWAY 6.9**: An array-length specification must be strictly positive.
**TAKEAWAY 6.10**: If the length is not an integer constant expression, an array is a VLA.
**TAKEAWAY 6.11**: The length of an array A is (sizeof A)/(sizeof A[0]).

### Arrays as Function Parameters

- **Innermost dimension loss**: Array parameters lose dimensionality; `void f(int a[10][20])` treats `a` as `int(*)[20]`
- **Pass by reference behavior**: Arrays are effectively passed by reference; modifications affect original
- **[static N] syntax**: `void f(int a[static 10])` asserts minimum size of 10 elements at call site

**TAKEAWAY 6.12**: The innermost dimension of an array parameter to a function is lost.
**TAKEAWAY 6.13**: Don't use the sizeof operator on array parameters to functions.
**TAKEAWAY 6.14**: Array parameters behave as if the array is passed by reference.

### Strings: Special 0-Terminated Char Arrays

- **Definition**: String = array of `char` terminated by null character `\0`
- **Literals**: `"hello"` is a 6-element array: `'h','e','l','l','o','\0'`
- **Character set**: Basic ASCII (128 chars); platform-specific encoding assumed (usually ASCII)
- **String vs char array distinction**: Not all `char[]` are strings; `char s[3] = {'a','b','c'};` is not 0-terminated

**TAKEAWAY 6.15**: A string is a 0-terminated array of char.
**TAKEAWAY 6.16**: Using a string function with a non-string leads to program failure.

### String & Memory Functions from `<string.h>`

| Function                       | Purpose                                                      |
| ------------------------------ | ------------------------------------------------------------ |
| `strlen(s)`                    | Length of null-terminated string s                           |
| `strcpy(target, source)`       | Copy source string to target (both must be 0-terminated)     |
| `strdup(s)`, `strndup(s, len)` | Allocate and copy string (C23: strndup)                      |
| `strcmp(s0, s1)`               | Lexicographic comparison; result: negative/0/positive        |
| `strcoll(s0, s1)`              | Locale-aware comparison                                      |
| `strchr(s, c)`                 | Find character c in string s                                 |
| `strspn(s0, s1)`               | Length of initial segment of s0 containing chars from s1     |
| `strcspn(s0, s1)`              | Length of initial segment of s0 NOT containing chars from s1 |
| `memcpy(target, source, len)`  | Copy len bytes (arrays can be non-0-terminated)              |
| `memcmp(s0, s1, len)`          | Compare len bytes lexicographically                          |
| `memchr(s, c, len)`            | Find byte c in first len bytes of s                          |

## 6.2 Pointers: Opaque References to Objects

### Pointer Basics

- **Definition**: Pointers hold memory addresses; their binary representation is platform-dependent
- **Opaque type principle**: Pointers are only manipulated via C-defined operations, not directly interpreted
- **Syntax**: `type* ptr;` declares pointer; `typename const*const ptr = value;` adds const qualifiers

**TAKEAWAY 6.17**: Pointers are opaque objects.

### Pointer States

- **Valid**: Points to an allocated object
- **Null**: Explicitly set to `nullptr` (C23) or `NULL`; evaluates false in logical expressions
- **Invalid/indeterminate**: Uninitialized or dangling; leads to undefined behavior

**TAKEAWAY 6.18**: Pointers are valid, null, or invalid.
**TAKEAWAY 6.19**: Initialization or assignment with nullptr makes a pointer null.
**TAKEAWAY 6.20**: In logical expressions, pointers evaluate to false if they are null.
**TAKEAWAY 6.21**: Invalid pointers lead to program failure.
**TAKEAWAY 6.22**: Always initialize pointers.

### Pointer Qualifiers

- **`const*`**: Pointer to constant data (can't modify through pointer)
- **`*const`**: Constant pointer (can't change pointer value)
- **`const*const`**: Both constant (immutable pointer to immutable data)

## 6.3 Structures: Aggregate Heterogeneous Objects

### Basic Structure Declaration

```c
struct tagName {
    type1 member1;
    type2 member2;
    // ...
};
```

- **Tags**: Optional identifier for the structure type
- **Members/fields**: Named components, declared like normal variables
- **Initialization**: Designated initializers: `struct s var = { .member = val, ... };`
- **Access**: Dot notation `var.member` for member access

**TAKEAWAY 6.23**: Omitted struct initializers force the corresponding member to 0.
**TAKEAWAY 6.24**: struct parameters are passed by value.
**TAKEAWAY 6.25**: Structures can be assigned.
**TAKEAWAY 6.26**: Structures cannot be compared with == or !=.

### Nested Structures

- **Nesting**: Members can be structs; allows hierarchical organization
- **Scope visibility**: Nested struct declarations have same visibility as outer struct
- **Layout**: Nested structures create additional abstraction level; gray boxes indicate padding

**TAKEAWAY 6.28**: All struct declarations in a nested declaration have the same scope of visibility.

### Structure Padding & Alignment

- **Padding**: Compiler inserts empty bytes between members for alignment to word boundaries
- **Alignment**: Fields aligned to efficient access positions; `char, short, int` have different requirements

**TAKEAWAY 6.27**: A structure layout is an important design decision.
**TAKEAWAY 6.29**: There can be padding after any structure member.
**TAKEAWAY 6.30**: There is no padding at the beginning of a structure.

**Optimization**: Reorder members by size (largest first) to minimize padding.

### Bit-Fields: Subword Storage

- **Syntax**: `type fieldname : bitwidth;` allocates specified number of bits
- **Coalescing**: Multiple consecutive bit-fields packed into single storage unit (compiler-dependent)
- **Limitations**: Sign issues with `int` bit-fields; type inference complications

**TAKEAWAY 6.31**: Do not use bare int for the specification of a bit-field.
**TAKEAWAY 6.32**: Use a \_BitInt(N) type for a numerical bit-field of width N.
**TAKEAWAY 6.33**: Use bool as type of a flag bit-field of width 1.

## 6.4 Unions: Overlay Different Types

### Union Basics (Brief)

- **Definition**: All members overlay in same memory location; only one member can hold value at a time
- **Storage**: Size = size of largest member
- **Use case**: Type punning, efficient storage sharing (rarely used in modern C)

## 6.5 typedef: Type Aliasing

### Creating Type Aliases

```c
typedef existing_type new_name;
typedef struct tagName structName;  // Alias for struct type
```

- **Not new types**: typedef creates alternative names for existing types, doesn't create new types
- **Common idiom**:
  ```c
  typedef struct nodeTag nodeTag;
  struct nodeTag { /* ... */ };
  ```
  Allows using `nodeTag` without `struct` keyword

**TAKEAWAY 6.34**: typedef introduces alternative names for types, not new types.

## Syntax/Types/Functions Dictionary

### Array-Related

| Construct                        | Purpose                                             |
| -------------------------------- | --------------------------------------------------- |
| `type name[N];`                  | Declare fixed array of N elements                   |
| `type name[N][M];`               | Declare 2D array                                    |
| `type name[];`                   | Declare array with inferred length from initializer |
| `name[i]`                        | Access element at index i                           |
| `sizeof(name) / sizeof(name[0])` | Calculate array length                              |
| `name[.N] = val`                 | Designated initializer at position N                |

### Pointer-Related

| Construct          | Purpose                          |
| ------------------ | -------------------------------- |
| `type* ptr;`       | Declare pointer to type          |
| `type const* ptr;` | Pointer to constant data         |
| `type* const ptr;` | Constant pointer to mutable data |
| `ptr = nullptr;`   | Assign null pointer (C23)        |
| `if (ptr) { }`     | Test if pointer is non-null      |

### Structure-Related

| Construct                  | Purpose                             |
| -------------------------- | ----------------------------------- |
| `struct tag { ... };`      | Define structure type               |
| `typedef struct tag tag;`  | Create alias without struct keyword |
| `var.member`               | Access member                       |
| `var = { .member = val };` | Designated initializer              |
| `sizeof(struct_var)`       | Total size (includes padding)       |

### String Functions (Core)

| Function | Signature                                                           |
| -------- | ------------------------------------------------------------------- |
| `strlen` | `size_t strlen(char const s[static 1]);`                            |
| `strcmp` | `signed strcmp(char const s0[static 1], char const s1[static 1]);`  |
| `strcpy` | `char* strcpy(char target[static 1], char const source[static 1]);` |
| `memcpy` | `void* memcpy(void* target, void const* source, size_t len);`       |
| `memcmp` | `signed memcmp(void const* s0, void const* s1, size_t len);`        |

## Danger Zone: Pitfalls & Undefined Behavior

### Array Pitfalls

1. **Array vs pointer confusion**: Arrays decay to pointers in many contexts but are distinct types
2. **Bounds violations**: Accessing `a[N]` where `a` has N elements causes UB
3. **sizeof on parameters**: `sizeof(arr_param)` gives pointer size, not array size
4. **VLA lifetime**: VLAs deallocate when exiting scope; dangling pointers result

### String Pitfalls

1. **Non-0-terminated arrays**: Passing non-string to `strlen()` causes scan past boundary → segfault
2. **Buffer overflow**: `strcpy()` writes beyond target; use bounded versions or `memcpy()`
3. **String comparison**: `==` compares pointer addresses, not content; use `strcmp()`
4. **Literal immutability**: String literals are read-only; `"hello"[0] = 'H'` is UB

### Pointer Pitfalls

1. **Uninitialized pointers**: `type* p;` contains indeterminate value; dereferencing is UB
2. **Null dereference**: `*nullptr` causes segmentation fault
3. **Dangling pointers**: Pointers to stack objects become invalid when scope exits
4. **Type aliasing violations**: `(int*)&double_obj` reinterprets bytes; violates aliasing rules

### Structure Pitfalls

1. **Padding assumptions**: Don't assume struct layout; padding varies by platform/compiler
2. **Bit-field type issues**: Signed bit-fields may overflow; use `unsigned` or `_BitInt`
3. **Assignment vs initialization**: struct members not explicitly initialized in partial initializer are set to 0, not left indeterminate
4. **Incomplete struct forward declarations**: `struct forward;` must be completed before dereferencing members

## Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* 1. DERIVED DATA TYPES SHOWCASE */

/* Arrays: Homogeneous grouping */
void demo_arrays(void) {
    printf("=== ARRAYS ===\n");

    /* Fixed-length array (CLA) */
    double temps[5] = { 20.0, 21.5, 19.3, 22.1, 20.8 };
    size_t len = sizeof(temps) / sizeof(temps[0]);
    printf("Daily temps (count=%zu): ", len);
    for (size_t i = 0; i < len; ++i) {
        printf("%.1f ", temps[i]);
    }
    printf("\n");

    /* Multidimensional array */
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    printf("Matrix[1][2] = %d\n", matrix[1][2]);

    /* Designated initializers */
    int sparse[10] = { [2] = 5, [7] = 99 };
    printf("sparse[2]=%d, sparse[7]=%d, sparse[0]=%d (default)\n",
           sparse[2], sparse[7], sparse[0]);
}

/* Strings: Special 0-terminated char arrays */
void demo_strings(void) {
    printf("\n=== STRINGS ===\n");

    /* String declaration and operations */
    char name[] = "Modern C";  /* Automatically 0-terminated */
    printf("String: \"%s\"\n", name);
    printf("Length: %zu\n", strlen(name));

    /* String comparison */
    char greeting[20];
    strcpy(greeting, "Hello, ");
    printf("After strcpy: \"%s\"\n", greeting);

    /* String functions */
    const char* source = "Device:Hostname:Port";
    char buffer[256];
    strcpy(buffer, source);

    /* Manual parsing without strtok (safer) */
    const char* ptr = buffer;
    int part = 0;
    for (size_t i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] == ':') {
            buffer[i] = '\0';
            printf("Part %d: \"%s\"\n", part++, ptr);
            ptr = &buffer[i + 1];
        }
    }
    printf("Part %d: \"%s\"\n", part, ptr);

    /* memcpy for non-0-terminated data */
    unsigned char raw_data[6] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x21 };
    char text[10] = { };
    memcpy(text, raw_data, 5);
    text[5] = '\0';
    printf("Raw bytes as text: \"%s\"\n", text);
}

/* Pointers: Opaque references (brief introduction) */
void demo_pointers(void) {
    printf("\n=== POINTERS (Introduction) ===\n");

    /* Valid pointer */
    int value = 42;
    int const* ptr_to_value = &value;
    printf("Pointed value: %d\n", *ptr_to_value);

    /* Null pointer */
    int* null_ptr = nullptr;
    printf("Null pointer is %s\n", (null_ptr ? "valid" : "null"));

    /* Pointer to string literal */
    char const* string_ptr = "C23 Standard";
    printf("String pointer points to: \"%s\"\n", string_ptr);
}

/* Structures: Aggregate heterogeneous data */
void demo_structures(void) {
    printf("\n=== STRUCTURES ===\n");

    /* Simple structure */
    struct Date {
        unsigned char day;     /* 1-31: 1 byte */
        unsigned char month;   /* 1-12: 1 byte */
        int16_t year;         /* Full year: 2 bytes */
    };                         /* Total: 4 bytes (no padding typically) */

    struct Date today = {
        .day = 7,
        .month = 4,
        .year = 2026
    };
    printf("Today: %u/%u/%d\n", today.day, today.month, today.year);

    /* Nested structures */
    struct Person {
        char name[50];
        struct DateOfBirth {
            struct Date date;
            unsigned char hour;
        } dob;
    };

    struct Person alice = {
        .name = "Alice",
        .dob = {
            .date = { 15, 3, 1990 },
            .hour = 14
        }
    };
    printf("Person: %s, DOB: %u/%u/%d @ %02u:00\n",
           alice.name, alice.dob.date.day, alice.dob.date.month,
           alice.dob.date.year, alice.dob.hour);

    /* Structure assignment */
    struct Date copy = today;
    printf("Copied date: %u/%u/%d\n", copy.day, copy.month, copy.year);

    /* Bit-fields for space efficiency */
    struct TimeCompact {
        unsigned char second : 6;  /* 0-59: 6 bits */
        unsigned char minute : 6;  /* 0-59: 6 bits */
        unsigned char hour : 5;    /* 0-23: 5 bits */
    };                             /* Total: 2 bytes vs 3 without bit-fields */

    struct TimeCompact now = { .second = 45, .minute = 30, .hour = 14 };
    printf("Compact time: %02u:%02u:%02u (uses ~2 bytes)\n",
           now.hour, now.minute, now.second);
}

/* Arrays of structures */
void demo_array_of_structs(void) {
    printf("\n=== ARRAYS OF STRUCTURES ===\n");

    struct Student {
        char name[30];
        float gpa;
        int id;
    };

    struct Student roster[3] = {
        { .name = "Alice", .gpa = 3.9f, .id = 101 },
        { .name = "Bob", .gpa = 3.7f, .id = 102 },
        { .name = "Carol", .gpa = 3.95f, .id = 103 }
    };

    printf("Roster:\n");
    for (size_t i = 0; i < 3; ++i) {
        printf("  %s (ID: %d, GPA: %.2f)\n",
               roster[i].name, roster[i].id, roster[i].gpa);
    }
}

/* typedef: Creating type aliases */
void demo_typedef(void) {
    printf("\n=== TYPEDEF ===\n");

    typedef unsigned int uint;
    typedef struct {
        uint width;
        uint height;
    } Dimensions;

    Dimensions screen = { .width = 1920, .height = 1080 };
    printf("Screen: %u x %u\n", screen.width, screen.height);
}

/* Function that modifies struct passed by value */
struct ColorRGB add_brightness(struct ColorRGB color, unsigned char delta) {
    /* Modifications affect only local copy, not original */
    color.red = (color.red + delta > 255) ? 255 : color.red + delta;
    color.green = (color.green + delta > 255) ? 255 : color.green + delta;
    color.blue = (color.blue + delta > 255) ? 255 : color.blue + delta;
    return color;  /* Return modified copy */
}

struct ColorRGB {
    unsigned char red, green, blue;
};

void demo_struct_passbyvalue(void) {
    printf("\n=== STRUCT PASS-BY-VALUE ===\n");

    struct ColorRGB original = { .red = 200, .green = 100, .blue = 50 };
    printf("Original color: R=%u, G=%u, B=%u\n",
           original.red, original.green, original.blue);

    struct ColorRGB brightened = add_brightness(original, 30);
    printf("After brightening: R=%u, G=%u, B=%u\n",
           brightened.red, brightened.green, brightened.blue);
    printf("Original unchanged: R=%u, G=%u, B=%u\n",
           original.red, original.green, original.blue);
}

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  Chapter 6: Derived Data Types - C23 Comprehensive Demo   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    demo_arrays();
    demo_strings();
    demo_pointers();
    demo_structures();
    demo_array_of_structs();
    demo_typedef();
    demo_struct_passbyvalue();

    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  Demo Complete                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    return 0;
}
```

## Summary of Key TAKEAWAYs

1. **Arrays & Pointers**: Arrays ≠ pointers; arrays decay in some contexts
2. **Memory Management**: VLAs restricted to function scope; no initialization
3. **Strings**: Always 0-terminated; string functions assume 0-termination
4. **Pointers**: Initialize all pointers; distinguish valid/null/invalid states
5. **Structures**: Aggregate heterogeneous data; members passed by value; padding exists
6. **Bit-fields**: Use `unsigned` or `_BitInt`; avoid plain `int`
7. **typedef**: Creates aliases, not new types; improves code readability
