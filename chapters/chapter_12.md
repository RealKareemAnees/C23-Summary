# Chapter 12: The C Memory Model

Source: 12-memorymodel.md

## 1. Exhaustive Concept Breakdown

### 1.1 Object Representation and Byte Composition

The C memory model treats all objects as collections of bytes. This fundamental abstraction enables:

- **Uniform byte-level access**: Every object can be viewed as an array of unsigned chars (`unsigned char[sizeof A]`).
- **Portable abstraction**: Programs do not need to know physical memory addresses; C provides virtual memory abstraction.
- **Safe isolation**: Reading/writing virtual memory owned by your process doesn't affect the OS or other processes.

The `sizeof` operator measures object size in bytes. Three character types occupy exactly 1 byte by definition: `char`, `unsigned char`, and `signed char`. These form the atomic building blocks of all other types.

TAKEAWAY 12.1: Pointer types with distinct base types are distinct.
TAKEAWAY 12.2: `sizeof(char)` is 1 by definition.
TAKEAWAY 12.3: Every object A can be viewed as `unsigned char[sizeof A]`.
TAKEAWAY 12.4: Pointers to character types are special.
TAKEAWAY 12.5: Use `char` for character and string data.
TAKEAWAY 12.6: Use `unsigned char` as the atom of all object types.
TAKEAWAY 12.7: The `sizeof` operator can be applied to objects and types.
TAKEAWAY 12.8: The size of all objects of type T is given by `sizeof(T)`.

### 1.2 Unions for Object Inspection

Unions overlay multiple type interpretations on the same object representation:

```c
typedef union {
    unsigned val;
    unsigned char bytes[sizeof(unsigned)];
} unsignedInspect;
```

This allows byte-level inspection of any object. A union doesn't collect objects into a larger structure but rather provides multiple type views of identical memory.

TAKEAWAY 12.9: The in-memory order of representation digits is implementation-defined (endianness).
TAKEAWAY 12.10: On most architectures, CHAR_BIT is 8 and UCHAR_MAX is 255.

**Endianness**: Little-endian stores low-order bytes first; big-endian stores high-order bytes first. C23's `<stdbit.h>` provides macros to query platform endianness.

### 1.3 Memory State and Aliasing

Objects constitute the state of the abstract machine. Multiple pointers can access the same object—aliasing—which complicates optimization:

```c
double blub(double const* a, double* b) {
    double myA = *a;
    *b = 2*myA;
    return *a;  // May differ from myA if a == b (aliasing)
}
```

TAKEAWAY 12.11 (ALIASING): With the exclusion of character types, only pointers of the same base type may alias.

This strict rule enables compiler optimizations by guaranteeing that a `size_t*` cannot alias a `double*`.

TAKEAWAY 12.12: Avoid the `&` operator (reduces aliasing opportunities).

### 1.4 Void Pointers and Untyped Access

`void*` strips type information from object pointers, enabling generic programming:

TAKEAWAY 12.13: Any object pointer converts to and from `void*` (function pointers excluded).
TAKEAWAY 12.14: An object has storage, type, and value.
TAKEAWAY 12.15: Converting object pointer to `void*` and back to the same type is the identity operation.
TAKEAWAY 12.16: Avoid `void*` (completely removes type information).

The sole value lost in `void*` conversion is type information; the address value itself remains intact.

### 1.5 Explicit Type Conversions (Casts)

Casts explicitly convert pointer types, often used for byte-level inspection:

```c
double X;
unsigned char* Xp = (unsigned char*)&X;  // Explicit cast required
```

TAKEAWAY 12.17: Don't use casts (deprive yourself of compiler type-safety).

Casts from "pointer to object" to "pointer to character type" are mostly harmless. Other casts hide design flaws and are error-prone.

### 1.6 Effective Types and Type Restrictions

Objects must be accessed through their declared effective type or through character-type pointers. This restriction prevents dangerous type punning:

TAKEAWAY 12.18 (EFFECTIVE TYPE): Objects must be accessed through their effective type or character-type pointers.
TAKEAWAY 12.19: Any union member can be accessed at any time (provided bytes represent a valid value).
TAKEAWAY 12.20: The effective type of a variable is the type of its declaration.
TAKEAWAY 12.21: Variables must be accessed through their declared type or character-type pointers.

**Asymmetry**: An array of `unsigned char` cannot be accessed as another type via cast, but a union with both types can overlay both interpretations.

### 1.7 Alignment Constraints

Alignment defines the byte boundaries at which objects of a type can start. Misalignment causes bus errors and crashes:

```c
complex double z;  // May require 16-byte alignment
unsigned char* buf = ...;
complex double* misaligned = (complex double*)&buf[4];  // CRASH
```

The `alignof(T)` operator (or `_Alignof` pre-C23) queries a type's alignment requirement. The `alignas` specifier (or `_Alignas`) forces specific alignment:

```c
alignas(sizeof(float[4])) float fvec[4];  // Vector-aligned allocation
```

TAKEAWAY 12.22: Objects of most types can't start at arbitrary byte positions; they require word-boundary alignment.

## 2. Syntax, Types, and Functions Dictionary

### Operators and Keywords

| Operator/Keyword     | Purpose                                                                       |
| -------------------- | ----------------------------------------------------------------------------- |
| `sizeof(T)`          | Size in bytes (applies to objects and types)                                  |
| `alignof(T)`         | Alignment requirement (C23: `alignof`; pre-C23: `_Alignof` in `<stdalign.h>`) |
| `alignas(T)`         | Force alignment to type T (C23: `alignas`; pre-C23: `_Alignas`)               |
| `(T)expr`            | Explicit cast of expression to type T                                         |
| `__STDC_ISO_10646__` | Macro: indicates `wchar_t` uses Unicode code points                           |

### Type System

| Type            | Purpose                                                       |
| --------------- | ------------------------------------------------------------- |
| `void*`         | Untyped generic pointer (strips type info)                    |
| `unsigned char` | Atomic byte type (all objects viewable as arrays of this)     |
| `ptrdiff_t`     | Signed integer for pointer differences                        |
| `uintptr_t`     | Unsigned integer large enough to hold any pointer (if exists) |

### Library Functions

| Function    | Header       | Purpose                     |
| ----------- | ------------ | --------------------------- |
| `memcpy()`  | `<string.h>` | Copy non-overlapping memory |
| `memmove()` | `<string.h>` | Copy with possible overlap  |
| `memchr()`  | `<string.h>` | Search for byte in memory   |
| `sizeof()`  | Operator     | Compute object/type size    |

### Constants

| Constant    | Purpose                                          |
| ----------- | ------------------------------------------------ |
| `CHAR_BIT`  | Number of bits in `char` (typically 8)           |
| `UCHAR_MAX` | Maximum value of `unsigned char` (typically 255) |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Alignment Mismatches

**Pitfall**: Casting unaligned pointers to stricter-alignment types.

```c
unsigned char* buf = ...;
int* p = (int*)&buf[1];  // Misaligned! Bus error possible.
*p = 42;  // CRASH
```

**Fix**: Use `aligned_alloc()` or ensure alignment via union overlays.

### 3.2 Effective Type Violations

**Pitfall**: Accessing objects through incorrect types via void-pointer casts.

```c
unsigned char A[sizeof(unsigned)] = { 9 };
unsigned* p = (unsigned*)A;  // UNDEFINED! Effective type != access type
int val = *p;  // UB
```

**Fix**: Use unions for intentional type overlaying, or access only as `unsigned char*`.

### 3.3 Aliasing Without Restriction

**Pitfall**: Assuming pointers of different base types never alias.

```c
void modify(size_t* a, double* b) {
    size_t myA = *a;
    *b = 2.0 * *a;  // Compiler assumes no aliasing by type rule
    // myA may be invalid if a == b!
}
modify(&e, (double*)&e);  // Type-punning call: UNDEFINED
```

**Fix**: Don't circumvent type system; use `restrict` keyword if needed.

### 3.4 Object Representation Access Without Proper Cast

**Pitfall**: Accessing object bytes assumes deep understanding of endianness/representation.

```c
int x = 42;
unsigned char* p = (unsigned char*)&x;  // OK for byte inspection
// But interpretation depends on endianness and padding
```

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* === 4.1 Object Representation Inspection via Unions === */

typedef union {
    uint32_t val;
    unsigned char bytes[sizeof(uint32_t)];
} uint32Inspect;

void inspect_endianness(void) {
    printf("\n--- Endianness Inspection ---\n");

    uint32Inspect num = { .val = 0x12345678 };
    printf("Value: 0x%X\n", num.val);

    printf("Byte order (first to last): ");
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        printf("0x%02X ", num.bytes[i]);
    }
    printf("\n");

    if (num.bytes[0] == 0x78) {
        printf("-> Little-endian system\n");
    } else {
        printf("-> Big-endian system\n");
    }
}

/* === 4.2 Alignment Requirements === */

void demonstrate_alignment(void) {
    printf("\n--- Alignment Information ---\n");

    printf("Alignment of char:           %zu bytes\n", alignof(char));
    printf("Alignment of short:          %zu bytes\n", alignof(short));
    printf("Alignment of int:            %zu bytes\n", alignof(int));
    printf("Alignment of long long:      %zu bytes\n", alignof(long long));
    printf("Alignment of double:         %zu bytes\n", alignof(double));
    printf("Alignment of int*:           %zu bytes\n", alignof(int*));
}

/* === 4.3 Forced Alignment with alignas === */

void demonstrate_aligned_storage(void) {
    printf("\n--- Forced Alignment ---\n");

    // Force alignment to double size
    alignas(sizeof(double) * 4) char buffer[64];

    printf("buffer alignment: %zu bytes (requested: %zu)\n",
           (size_t)((uintptr_t)buffer % 16 == 0 ? 16 : 8),
           alignof(double) * 4);
}

/* === 4.4 Void Pointer Generic Programming === */

typedef struct {
    int id;
    char name[32];
} Person;

void process_generic(void* obj, const char* type_name) {
    printf("\n--- Generic Processing ---\n");
    printf("Processing %s object at %p\n", type_name, obj);

    // Cast back to known type
    if (strcmp(type_name, "Person") == 0) {
        Person* p = (Person*)obj;
        printf("  ID: %d, Name: %s\n", p->id, p->name);
    }
}

/* === 4.5 Pointer Arithmetic and Effective Types === */

void demonstrate_pointer_arithmetic(void) {
    printf("\n--- Pointer Arithmetic ---\n");

    int arr[5] = { 10, 20, 30, 40, 50 };
    int* p = &arr[0];

    printf("Array address: %p\n", (void*)p);
    printf("Element at p[0]: %d\n", *p);
    printf("Element at p[2]: %d\n", p[2]);

    // Pointer difference
    int* end = &arr[4];
    ptrdiff_t diff = end - p;
    printf("Difference (end - start): %td elements\n", diff);
}

/* === 4.6 Union-based Type Overlay (Safe) === */

typedef union {
    struct {
        uint16_t low;
        uint16_t high;
    } parts;
    uint32_t full;
} uint32Parts;

void demonstrate_union_overlay(void) {
    printf("\n--- Union Type Overlay ---\n");

    uint32Parts num = { .full = 0xDEADBEEF };
    printf("Full value: 0x%X\n", num.full);
    printf("Low 16 bits: 0x%X\n", num.parts.low);
    printf("High 16 bits: 0x%X\n", num.parts.high);
}

/* === 4.7 Byte-level Inspection (Proper Cast Usage) === */

void inspect_double_bytes(double d) {
    printf("\n--- Double Byte Inspection ---\n");
    printf("Double value: %g\n", d);
    printf("Byte representation: ");

    // PROPER cast: from object pointer to unsigned char pointer
    unsigned char* bytes = (unsigned char*)&d;
    for (size_t i = 0; i < sizeof(double); ++i) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}

/* === 4.8 Aliasing Rules Demonstration === */

void demonstrate_aliasing(void) {
    printf("\n--- Aliasing Rules ---\n");

    // Same-type pointers CAN alias
    int x = 10;
    int y = 20;
    int* p1 = &x;
    int* p2 = &y;
    printf("p1 (%p) and p2 (%p) are different: %s\n",
           (void*)p1, (void*)p2,
           p1 != p2 ? "yes (aliasing allowed)" : "no");

    // Different-type pointers CANNOT alias (compiler optimization)
    int i = 42;
    double d = 3.14;
    int* pi = &i;
    double* pd = &d;
    printf("int* and double* are different: yes (no aliasing rule)\n");
}

/* === 4.9 Effective Type Restrictions === */

void demonstrate_effective_types(void) {
    printf("\n--- Effective Type Rules ---\n");

    int x = 100;
    printf("Variable x (int): %d\n", x);

    // Valid: access through declared type
    printf("Access via int*: %d\n", *(int*)&x);

    // Valid: access as unsigned char array
    printf("Access as bytes: ");
    unsigned char* bytes = (unsigned char*)&x;
    for (size_t j = 0; j < sizeof(int); ++j) {
        printf("%02X ", bytes[j]);
    }
    printf("\n");

    // INVALID (UB): access through different type
    // printf("Access via double*: %g\n", *(double*)&x);  // DON'T
}

int main(void) {
    printf("=== Chapter 12: C Memory Model ===\n");

    inspect_endianness();
    demonstrate_alignment();
    demonstrate_aligned_storage();

    Person alice = { 1, "Alice" };
    process_generic(&alice, "Person");

    demonstrate_pointer_arithmetic();
    demonstrate_union_overlay();

    inspect_double_bytes(3.14159);
    demonstrate_aliasing();
    demonstrate_effective_types();

    printf("\n=== End Chapter 12 ===\n");
    return 0;
}
```

**Key Takeaways from Example**:

- Endianness varies by platform; use unions to inspect safely.
- Alignment affects where objects start in memory; misalignment crashes.
- `void*` enables generic APIs but loses type safety; use carefully.
- Effective type rule: access only through declared type or `unsigned char*`.
- Aliasing restrictions: same-base-type pointers may alias; different-type pointers don't.
- Unions safely overlay types; explicit casts are dangerous.
