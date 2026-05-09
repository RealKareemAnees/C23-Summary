# Chapter 18: Type-Generic Programming

Source: 18-typegenericprogramming.md

## 1. Exhaustive Concept Breakdown

### 1.1 Inherent Type-Generic Features

C's operators are type-generic: `==`, `+`, `*` work across integer, floating-point, pointer types with automatic conversions.

**Binary operators (permit equal types)**:

| Operator             | Integer | Float | Complex | Pointer                |
| -------------------- | ------- | ----- | ------- | ---------------------- |
| `==`, `!=`           | ✓       | ✓     | ✓       | ✓                      |
| `-`                  | ✓       | ✓     | ✓       | ✓ (pointer difference) |
| `+`, `*`, `/`        | ✓       | ✓     | ✓       | —                      |
| `<`, `<=`, `>=`, `>` | ✓       | ✓     | —       | —                      |
| `%`, `^`, `&` \|     | ✓       | —     | —       | —                      |

**Default promotions and conversions** (Figure 18.1):

- Narrow integers promoted to wider before arithmetic.
- Float arguments to variadic functions promoted to double.
- Arithmetic conversion determines common type for mixed-type operations.

### 1.2 Macros for Type-Genericity

**Expression macros**:

```c
#define GRAY(R, G, B) (((R)+(G)+(B))/3)  // Works for any numeric type
```

**Declaration macros**:

```c
#define declareColorType(N) typedef struct N N
#define defineColorStruct(N, T) struct N { T r; T g; T b; }

declareColorType(ColorRGB);
defineColorStruct(ColorRGB, uint8_t);
```

**Statement macros** (safe with do-while-false):

```c
#define dataCondStore(T, BASE, P, E, D) \
    do { \
        T* _pr_p = (P); \
        BASE _pr_expected = (E); \
        BASE _pr_desired = (D); \
        bool _pr_c; \
        do { \
            mtx_lock(&_pr_p->mtx); \
            _pr_c = (_pr_p->data == _pr_expected); \
            if (_pr_c) _pr_p->data = _pr_desired; \
            mtx_unlock(&_pr_p->mtx); \
        } while(!_pr_c); \
    } while(false)
```

### 1.3 Variadic Functions (Mostly Obsolete)

**`printf` family**: Variadic; format string specifies argument types.

TAKEAWAY 18.1: Modern compilers check printf format strings; user variadic functions are unchecked and dangerous.

```c
int printf(char const format[static 1], ...);  // ... = variable args
```

**Argument conversion**: Narrow integers and `float` are promoted (no `float` in `...`).

**`va_list` interface** (`<stdarg.h>`):

```c
void vprintf_wrapper(char const fmt[static 1], ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
```

### 1.4 Function Pointers for Type-Genericity

**Comparison function pattern**:

```c
typedef int compar_t(void const*, void const*);
int qsort(void* base, size_t nmemb, size_t size, compar_t* compare);
```

Enables same sorting function for any type, with custom comparator.

### 1.5 void Pointers in Generic Interfaces

`void*` strips type information; user responsible for type safety.

TAKEAWAY 18.2: The disadvantage of void-pointer interfaces is loss of type safety.

### 1.6 Type-Generic C Library Functions

**`<tgmath.h>` (C99+)**: Type-generic math macros.

```c
#define cos(x) _Generic((x), \
    float: cosf(x), \
    double: cos(x), \
    long double: cosl(x) \
)
```

**C23 type-safe search functions**:

```c
char const str[] = "hello";
char const needle[] = "ll";
char const* pos = strstr(str, needle);  // Returns char const*

char mut_str[] = "hello";
char* mpos = strstr(mut_str, needle);   // Returns char*
```

These via `_Generic` preserve `const` qualification.

### 1.7 `_Generic` Expression (C11+)

Compile-time type selection:

```c
#define print_any(x) _Generic((x), \
    int: printf("int: %d", (x)), \
    float: printf("float: %f", (x)), \
    double: printf("double: %f", (x)), \
    default: printf("unknown type") \
)
```

Selects branch based on compile-time type of `x`.

### 1.8 Type Inference (C23)

**`auto` keyword**: Deduces type from initializer.

```c
auto x = 42;              // int
auto y = 3.14;            // double
auto z = "hello";         // char const*
auto arr = (int[]){1,2,3};  // int*
```

**`typeof` and `typeof_unqual`** (C23):

```c
int x = 10;
typeof(x) y = 20;           // y is int
typedef typeof(x) int_t;    // int_t = int

const int cx = 10;
typeof(cx) cy = 20;         // cy is const int
typeof_unqual(cx) cy2 = 20; // cy2 is int (unqualified)
```

Enables generic-like programming without `_Generic` combinatorial explosion.

## 2. Syntax, Types, and Functions Dictionary

### Type-Generic Mechanisms

| Mechanism                          | Purpose                                  |
| ---------------------------------- | ---------------------------------------- |
| Operators (`+`, `-`, `*`)          | Type-generic via promotion/conversion    |
| `_Generic(expr, type: value, ...)` | Compile-time type selection (C11+)       |
| `auto`                             | Type deduction from initializer (C23)    |
| `typeof(expr)`                     | Extract type of expression (C23)         |
| `typeof_unqual(expr)`              | Extract type, remove qualifiers (C23)    |
| Function pointers                  | Runtime type-generic via callbacks       |
| `void*`                            | Untyped pointer (manual type management) |

### Library Functions

| Function                                  | Header       | Purpose                                      |
| ----------------------------------------- | ------------ | -------------------------------------------- |
| `qsort(base, nmemb, size, compar)`        | `<stdlib.h>` | Generic sort (requires compar function)      |
| `bsearch(key, base, nmemb, size, compar)` | `<stdlib.h>` | Generic binary search                        |
| `cos(x)`, `sin(x)`, etc. (macros)         | `<tgmath.h>` | Type-generic math (float/double/long double) |
| `strstr(s, sub)` (type-generic)           | `<string.h>` | Type-generic search (C23)                    |

### Variadic Support (`<stdarg.h>`)

| Macro/Type                 | Purpose                           |
| -------------------------- | --------------------------------- |
| `va_list`                  | Type for argument list state      |
| `va_start(ap, last_param)` | Initialize argument list iterator |
| `va_arg(ap, type)`         | Fetch next argument as type       |
| `va_end(ap)`               | Cleanup (some platforms)          |
| `va_copy(dest, src)`       | Copy argument list state          |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Type Mismatch in `_Generic`

**Pitfall**: No coverage for actual type used.

```c
#define my_func(x) _Generic((x), int: do_int(x), char: do_char(x))
my_func(3.14);  // double not in _Generic, default case missing → error
```

**Fix**: Always include `default:` or cover all possible types.

### 3.2 `typeof` and Qualifiers

**Pitfall**: `typeof` includes qualifiers; may not behave as expected.

```c
const int x = 10;
typeof(x) y = 20;  // y is const int, cannot rebind!
```

**Fix**: Use `typeof_unqual(x)` to strip qualifiers.

### 3.3 `void*` Type Unsafety

**Pitfall**: Casting `void*` to wrong type.

```c
int* p = malloc(sizeof(int));
*((double*)p) = 3.14;  // Dereferencing as double UB
```

**Fix**: Document pointer types; avoid void pointers when possible.

### 3.4 Variadic Function Unchecked Arguments

**Pitfall**: User-written variadic function with no compile-time checking.

```c
void my_printf(char const fmt[static 1], ...) {
    // No compiler verification of format string
    // User can pass wrong types
}
my_printf("%d", 3.14);  // Compiles; runtime error likely
```

**Fix**: Use library functions with format string checking; avoid user variadic functions.

### 3.5 `auto` Type Inference Surprises

**Pitfall**: `auto` may deduce unexpected type.

```c
auto x = malloc(10);     // x is void*, not int* (need explicit cast)
int arr[5] = {1,2,3,4,5};
auto p = arr;            // p is int*, not int[5] (array decay)
```

**Fix**: Be explicit with types; use `auto` cautiously.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>
#include <stdarg.h>

/* === 4.1 _Generic for Type-Specific Handling === */

#define print_value(x) _Generic((x), \
    int: printf("int: %d\n", (x)), \
    float: printf("float: %f\n", (x)), \
    double: printf("double: %f\n", (x)), \
    char*: printf("string: %s\n", (x)), \
    default: printf("unknown type\n") \
)

void demo_generic(void) {
    printf("\n--- _Generic Compile-Time Selection ---\n");
    print_value(42);
    print_value(3.14f);
    print_value(2.718);
    print_value("Generic!");
}

/* === 4.2 Type-Generic Math Functions === */

void demo_tgmath(void) {
    printf("\n--- Type-Generic Math (<tgmath.h>) ---\n");

    // tgmath.h macros dispatch to float/double/long double versions
    float f = 0.5f;
    double d = 0.5;
    long double ld = 0.5L;

    printf("sin(float 0.5) = %f\n", sin(f));      // calls sinf()
    printf("sin(double 0.5) = %f\n", sin(d));     // calls sin()
    printf("sin(long double 0.5) = %Lf\n", sin(ld));  // calls sinl()
}

/* === 4.3 Type Inference (C23) === */

void demo_auto(void) {
    printf("\n--- Type Inference (auto, C23) ---\n");

    auto x = 42;                    // int
    auto y = 3.14;                  // double
    auto z = "hello";               // char const*

    printf("x (auto) = %d\n", x);
    printf("y (auto) = %f\n", y);
    printf("z (auto) = %s\n", z);

    // typeof for meta-programming
    typedef typeof(x) int_type;
    typedef typeof(y) double_type;

    int_type a = 100;
    double_type b = 2.5;
    printf("typeof(x): a = %d, typeof(y): b = %f\n", a, b);
}

/* === 4.4 typeof_unqual (C23) === */

void demo_typeof_unqual(void) {
    printf("\n--- typeof_unqual (Remove Qualifiers, C23) ---\n");

    const int cx = 10;
    volatile double vd = 3.14;

    // typeof preserves qualifiers
    typeof(cx) y1 = 20;      // y1 is const int
    // y1 = 30;  // Error: const

    // typeof_unqual strips qualifiers
    typeof_unqual(cx) y2 = 30;     // y2 is int
    typeof_unqual(vd) y3 = 2.71;   // y3 is double

    printf("y2 (unqualified) = %d\n", y2);
    printf("y3 (unqualified) = %f\n", y3);
}

/* === 4.5 Comparison Functions for qsort === */

typedef struct {
    int value;
    char name[20];
} Record;

int compare_records_by_value(void const* a, void const* b) {
    Record const* ra = (Record const*)a;
    Record const* rb = (Record const*)b;
    return (ra->value > rb->value) - (ra->value < rb->value);
}

void demo_qsort(void) {
    printf("\n--- Generic Sort (qsort) ---\n");

    Record records[] = {
        {30, "Charlie"},
        {10, "Alice"},
        {20, "Bob"}
    };

    size_t n = sizeof records / sizeof *records;

    printf("Before sort:\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  %d: %s\n", records[i].value, records[i].name);
    }

    qsort(records, n, sizeof *records, compare_records_by_value);

    printf("After sort:\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  %d: %s\n", records[i].value, records[i].name);
    }
}

/* === 4.6 Type-Safe String Search (C23) === */

void demo_type_generic_search(void) {
    printf("\n--- Type-Generic String Search (C23) ---\n");

    // Const string search returns const pointer
    char const cs[] = "hello world";
    char const* pos1 = strstr(cs, "world");
    if (pos1) printf("Found in const: %s\n", pos1);

    // Mutable string search returns mutable pointer
    char ms[] = "hello world";
    char* pos2 = strstr(ms, "world");
    if (pos2) {
        printf("Found in mutable: %s\n", pos2);
        pos2[0] = 'W';  // Can modify
        printf("Modified: %s\n", ms);
    }
}

/* === 4.7 Macro-Based Type Genericity === */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN_OF_THREE(a, b, c) MIN(MAX(a, b), c)

#define SWAP(a, b) \
    do { \
        typeof(a) _tmp = (a); \
        (a) = (b); \
        (b) = _tmp; \
    } while(0)

void demo_macro_genericity(void) {
    printf("\n--- Macro-Based Type Genericity ---\n");

    int i1 = 5, i2 = 10;
    SWAP(i1, i2);
    printf("After SWAP: i1=%d, i2=%d\n", i1, i2);

    double d1 = 1.5, d2 = 2.5;
    SWAP(d1, d2);
    printf("After SWAP: d1=%f, d2=%f\n", d1, d2);

    printf("MAX(3.5, 7.2) = %f\n", (double)MAX(3.5, 7.2));
}

/* === 4.8 Operator Type-Genericity (Conversions) === */

void demo_operator_genericity(void) {
    printf("\n--- Operator Type-Genericity ---\n");

    int i = 10;
    float f = 2.5f;
    double d = 3.0;

    // Operators work across types via conversions
    auto result1 = i + f;      // int + float → float
    auto result2 = f * d;      // float * double → double
    auto result3 = d / i;      // double / int → double

    printf("10 + 2.5f = %f\n", result1);
    printf("2.5f * 3.0 = %f\n", result2);
    printf("3.0 / 10 = %f\n", result3);
}

int main(void) {
    printf("=== Chapter 18: Type-Generic Programming ===\n");

    demo_generic();
    demo_tgmath();
    demo_auto();
    demo_typeof_unqual();
    demo_qsort();
    demo_type_generic_search();
    demo_macro_genericity();
    demo_operator_genericity();

    printf("\n=== End Chapter 18 ===\n");
    return 0;
}
```

**Key Takeaways**:

- C inherently provides type-genericity via operators and conversions.
- `_Generic` enables compile-time type-specific code paths.
- C23 `auto`, `typeof`, `typeof_unqual` streamline generic programming.
- `void*` and function pointers enable runtime type-genericity at cost of safety.
- Library functions increasingly use type-generic macros to preserve const-safety.
