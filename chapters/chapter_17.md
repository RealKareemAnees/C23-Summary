# Chapter 17: Function-like Macros

Source: 17-functionlikemacros.md

## 1. Exhaustive Concept Breakdown

### 1.1 Macro Replacement Fundamentals

TAKEAWAY 17.1: Whenever possible, prefer an inline function to a functional macro.

Function-like macros use textual substitution (preprocessing phase) before compilation. Inline functions provide type-safe alternatives with equivalent performance.

TAKEAWAY 17.2: A functional macro shall provide a simple interface to a complex task.

**Textual replacement mechanics**:

TAKEAWAY 17.3: Macro replacement is done in an early translation phase, before any other interpretation.

Preprocessing sees no types, keywords, or C semantics—only token streams. This enables:

- Type-generic interfaces
- Argument checking via string literal enforcement
- Variadic macro support
- Complex compile-time computations

### 1.2 Macro Expansion Rules

Functional macro invocation `MACRO(args)` follows strict rules:

1. Macro definition temporarily disabled (prevent infinite recursion).
2. Argument list scanned for parentheses/commas; each comma separates arguments.
3. Each argument recursively expanded for nested macros.
4. Argument text assigned to parameters.
5. Replacement text copied; all parameters replaced with expanded arguments.
6. Result subject to macro replacement again.
7. Final replacement text inserted at invocation point.
8. Macro definition re-enabled.

**Example**:

```c
#define MINSIZE(X, Y) (sizeof(X)<sizeof(Y) ? sizeof(X) : sizeof(Y))
#define BYTECOPY(T, S) memcpy(&(T), &(S), MINSIZE(T, S))
BYTECOPY(A, B)  // Expands with full parenthesization
```

**Macro retention** (C23/C11):

TAKEAWAY 17.4: If a functional macro is not followed by `()`, it is not expanded.

Enables coexistence of macro and function with same name:

```c
#define string_literal(S) string_literal("" S "")  // Macro
inline char const* (string_literal)(char const str[static 1]) {  // Function
    return str;
}
```

### 1.3 Argument Checking via Type Constraints

Macros cannot enforce types, but can enforce usage patterns:

**String literal enforcement**:

```c
#define TRACE_MSG(F) \
    do { fprintf(stderr, "" F ""); } while(0)

TRACE_MSG("debug: %d\n", x);      // "" "debug: %d\n" "" – OK
TRACE_MSG(fmt_from_user);          // Concatenation fails – compile error
```

Concatenation (`""` + user_string + `""`) works only for string literals.

### 1.4 Variadic Macros

**C89/C99 variadic functions**: `printf(fmt, ...)`

**C99/C23 variadic macros**: `#define MACRO(a, ...) ...` – variable argument list

```c
#define LOG(fmt, ...) \
    printf("[LOG] " fmt "\n", ##__VA_ARGS__)

LOG("value: %d", 42);      // "value: 42"
LOG("simple");              // "simple" (note: ##__VA_ARGS__ removes trailing comma)
```

### 1.5 Type-Generic Macros

Macros enable type-generic programming impossible with functions:

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))  // Works for any comparable type
#define SWAP(a, b) do { typeof(a) tmp = (a); (a) = (b); (b) = tmp; } while(0)
```

**Common pitfall**: Arguments evaluated multiple times.

```c
#define square_macro(X) (X*X)
int i = 0;
int y = square_macro(i++);  // i++ evaluated twice! i becomes 2.
```

**Fix**: Use inline functions or macro that evaluates arguments once (via local variables).

## 2. Syntax, Types, and Functions Dictionary

### Macro Directives

| Directive                    | Purpose                      |
| ---------------------------- | ---------------------------- |
| `#define MACRO body`         | Object-like macro (constant) |
| `#define MACRO(args) body`   | Function-like macro          |
| `#define MACRO(a, ...) body` | Variadic macro (C99+)        |
| `#undef MACRO`               | Undefine macro               |
| `#if defined(MACRO)`         | Conditional compilation      |

### Macro Operators

| Operator        | Purpose                                        |
| --------------- | ---------------------------------------------- |
| `#arg`          | Stringify argument (convert to string literal) |
| `arg1##arg2`    | Token concatenation                            |
| `##__VA_ARGS__` | Variadic args with trailing comma removal      |

### Special Macros

| Macro         | Purpose                              |
| ------------- | ------------------------------------ |
| `__DATE__`    | Compilation date (string)            |
| `__TIME__`    | Compilation time (string)            |
| `__LINE__`    | Current line number (integer)        |
| `__FILE__`    | Current file name (string)           |
| `__func__`    | Current function name (string, C99+) |
| `__VA_ARGS__` | Variadic macro arguments list        |

## 3. Danger Zone: Pitfalls and Undefined Behavior

### 3.1 Multiple Evaluation of Arguments

**Pitfall**: Macro arguments evaluated each reference.

```c
#define SQUARE(x) (x*x)
int n = 5;
int result = SQUARE(n++);  // n++ evaluated twice: n=7, result=25
```

**Fix**: Use inline function or store arguments in locals within macro.

### 3.2 Operator Precedence Errors

**Pitfall**: Missing parentheses in macro body.

```c
#define ADD(a, b) a + b
int z = ADD(1, 2) * 3;  // 1 + 2 * 3 = 7, not 9
```

**Fix**: Parenthesize arguments and result: `#define ADD(a, b) ((a)+(b))`

### 3.3 Side Effects in Macro Arguments

**Pitfall**: Function calls with side effects as macro arguments.

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int x = MAX(read_sensor(), read_sensor());  // Reads twice!
```

**Fix**: Compute arguments before macro invocation.

### 3.4 Global Variable Pollution

**Pitfall**: Macro parameter names clash with globals.

```c
#define LOOP(i, n) for (int i = 0; i < n; ++i)
int i = 100;  // Global
LOOP(i, 5) {    // Uses local i, not global
    // ...
}
// Macros don't respect scope; avoid common names
```

**Fix**: Use unique prefixes (`_IMPL_`, `_macro_`) or inline functions.

### 3.5 Type-Generic Macro Confusion

**Pitfall**: Macro works unexpectedly with different types.

```c
#define ABS(x) ((x) < 0 ? -(x) : (x))
unsigned u = ABS(-5);  // -5 is int, -(int) stays int, conversion to unsigned
```

**Fix**: Document type requirements; prefer type-safe functions with overloading or C23 `_Generic`.

## 4. Comprehensive C23 Working Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* === 4.1 Safe Macro with Parenthesization === */

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

void demo_safe_macros(void) {
    printf("\n--- Safe Macros with Parenthesization ---\n");
    printf("MAX(3, 7) = %d\n", MAX(3, 7));
    printf("MAX(3, 7) * 2 = %d (not 3 * 2)\n", MAX(3, 7) * 2);
    printf("ABS(-5) = %d\n", ABS(-5));
}

/* === 4.2 Variadic Macro === */

#define LOG(level, fmt, ...) \
    printf("[%s] " fmt "\n", level, ##__VA_ARGS__)

#define DEBUG(fmt, ...) LOG("DEBUG", fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) LOG("ERROR", fmt, ##__VA_ARGS__)

void demo_variadic(void) {
    printf("\n--- Variadic Macros ---\n");
    DEBUG("Starting process...");
    DEBUG("Value: %d, Name: %s", 42, "test");
    ERROR("Failed operation");
}

/* === 4.3 String Literal Enforcement === */

#define SAFE_PRINT(fmt) \
    do { \
        printf("Safe: " fmt " [from %s:%d]\n", __func__, __LINE__); \
    } while(0)

void demo_string_enforcement(void) {
    printf("\n--- String Literal Enforcement ---\n");
    SAFE_PRINT("Using string literal");
    // SAFE_PRINT(user_string);  // Compile error: can't concatenate with variable
}

/* === 4.4 do-while-false Idiom === */

#define SWAP(a, b) \
    do { \
        typeof(a) _tmp = (a); \
        (a) = (b); \
        (b) = _tmp; \
    } while(0)

void demo_swap(void) {
    printf("\n--- do-while-false Swap Idiom ---\n");

    int x = 10, y = 20;
    printf("Before: x=%d, y=%d\n", x, y);
    SWAP(x, y);
    printf("After: x=%d, y=%d\n", x, y);

    double a = 1.5, b = 2.5;
    SWAP(a, b);
    printf("Double swap: a=%f, b=%f\n", a, b);
}

/* === 4.5 Stringify Operator (#) === */

#define STR(x) #x
#define STR_VALUE(x) "Value of " #x " is: %d\n"

void demo_stringify(void) {
    printf("\n--- Stringify Operator ---\n");

    int value = 42;
    printf("STR(value) = %s\n", STR(value));
    printf(STR_VALUE(value), value);

    printf("STR(MAX) = %s\n", STR(MAX));  // Macro name stringified
}

/* === 4.6 Token Concatenation (##) === */

#define MAKE_PAIR_TYPE(T) \
    typedef struct { \
        T first; \
        T second; \
    } pair_ ## T

// Create specific pair types
MAKE_PAIR_TYPE(int);
MAKE_PAIR_TYPE(double);

void demo_concatenation(void) {
    printf("\n--- Token Concatenation ---\n");

    pair_int pi = { 1, 2 };
    pair_double pd = { 1.5, 2.5 };

    printf("pair_int: (%d, %d)\n", pi.first, pi.second);
    printf("pair_double: (%f, %f)\n", pd.first, pd.second);
}

/* === 4.7 Predefined Macros === */

void demo_predefined(void) {
    printf("\n--- Predefined Macros ---\n");
    printf("__FILE__: %s\n", __FILE__);
    printf("__LINE__: %d\n", __LINE__);
    printf("__func__: %s\n", __func__);
    printf("__DATE__: %s\n", __DATE__);
    printf("__TIME__: %s\n", __TIME__);
}

/* === 4.8 Conditional Compilation === */

#define DEBUG_MODE 1

void demo_conditional(void) {
    printf("\n--- Conditional Compilation ---\n");

#if DEBUG_MODE
    printf("Debug mode ENABLED\n");
#else
    printf("Debug mode DISABLED\n");
#endif

#ifdef __STDC_NO_VLA__
    printf("VLAs are NOT supported on this platform\n");
#else
    printf("VLAs are supported\n");
#endif
}

/* === 4.9 Type-Generic with _Generic (C11+) === */

#define print_value(x) \
    _Generic((x), \
        int: printf("int: %d\n", (x)), \
        double: printf("double: %f\n", (x)), \
        char*: printf("string: %s\n", (x)), \
        unsigned: printf("unsigned: %u\n", (x)) \
    )

void demo_generic(void) {
    printf("\n--- Type-Generic Macros ---\n");
    print_value(42);
    print_value(3.14);
    print_value("hello");
    print_value(100U);
}

int main(void) {
    printf("=== Chapter 17: Function-like Macros ===\n");

    demo_safe_macros();
    demo_variadic();
    demo_string_enforcement();
    demo_swap();
    demo_stringify();
    demo_concatenation();
    demo_predefined();
    demo_conditional();
    demo_generic();

    printf("\n=== End Chapter 17 ===\n");
    return 0;
}
```

**Key Takeaways**:

- Macros enable compile-time computation and type-genericity unavailable to functions.
- Always parenthesize macro arguments and results to avoid precedence errors.
- Do-while-false idiom allows use in statement contexts.
- String literal enforcement prevents format-string vulnerabilities.
- Prefer inline functions unless macro superpowers are needed.
